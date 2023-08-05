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

void reader::init(const std::string& name, const int count, writer* myWriter){
    in.open(name);
    threadCount = count;
    theWriter = myWriter;
    pthread_t threads[threadCount];
}

void reader::run() {

    for(size_t i = 0; i < threadCount; ++i) {
        if (pthread_create(threads + i, NULL, &runner, NULL) != 0){
            std::cout<<"There was an error creating thread Number "<<threadCount<<std::endl;
            exit(EXIT_FAILURE);
        }
    }

    for(size_t i = 0; i < threadCount; ++i) {
        if (pthread_join(threads[i], NULL) != 0){
            std::cout<<"There was an error joining thread Number "<<threadCount<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
        
}

void* reader::runner(void* arg) { 
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)){
            theWriter->append(line);
        }
        in.close();
    }
    else {
        std::cerr << "Error: Infile could not be opened" << std::endl;
    }
    return nullptr; 
    }
