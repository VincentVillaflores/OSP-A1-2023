/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"

#include "writer.h"

/**
 * implement the functions needed for this class
 **/
std::ifstream reader::in;
int reader::threadCount = 0;
writer* reader::theWriter;

void reader::init(const std::string& name, const int count, writer* myWriter){
    in.open(name);
    threadCount = count;
    theWriter = myWriter;
}

void reader::run() {
    if (in.is_open()) {
        pthread_t threads[threadCount];
        
        for (int i = 0; i < threadCount; i++){
            size_t tmp = i;
            if (pthread_create(threads + i++, NULL, &runner, (void*) tmp) != 0){
                std::cout<<"There was an error creating thread Number "<<threadCount<<std::endl;
                exit(EXIT_FAILURE);
            }
        }

        for(int i = 0; i < threadCount; i++) {
            if (pthread_join(threads[i], NULL) != 0){
                std::cout<<"There was an error joining thread Number "<<threadCount<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        in.close();
    }
    else {
        std::cerr << "Error: Infile could not be opened" << std::endl;
    }

    
        
}

void* reader::runner(void* arg) {
    while (in.good()){ 
        std::string line;
        getline(in,line);
        std::cout<<"Thread: "<<(size_t) arg<<" Line: "<<line<<std::endl;
        theWriter->append(line);
    }
    return nullptr; 
}