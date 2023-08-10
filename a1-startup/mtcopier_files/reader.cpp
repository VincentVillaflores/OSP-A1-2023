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
pthread_mutex_t reader::lock;

void reader::init(const std::string& name, const int count, writer* myWriter){
    std::cout<<"reader::init()"<<std::endl;
    in.open(name);
    threadCount = count;
    theWriter = myWriter;
}

void reader::run() {
    std::cout<<"reader::run()"<<std::endl;
    if (in.is_open()) {
        pthread_t threads[threadCount];
        pthread_mutex_init(&lock,0);

        for (int i = 0; i < threadCount; i++){
            size_t tmp = i;
            std::cout<<"i: "<<i<<std::endl;
            if (pthread_create(threads + i, NULL, &runner, (void*) tmp) != 0){
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
        pthread_mutex_destroy(&lock);
    }
    else {
        std::cerr << "Error: Infile could not be opened" << std::endl;
    }

    
        
}

void* reader::runner(void* arg) {
    std::cout<<"reader::runner()"<<std::endl;
    while (in.good()){ 
        pthread_mutex_lock(&lock);
        std::string line;
        getline(in,line);
        theWriter->append(line);
        std::cout<<"ReaderThread: "<<(size_t) arg<<" Line: "<<line<<std::endl;
        pthread_mutex_unlock(&lock);
    }
    return nullptr; 
}