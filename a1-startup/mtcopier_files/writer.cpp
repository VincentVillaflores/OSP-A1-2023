/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "writer.h"

#include "reader.h"

/**
 * implement these functions requred for the writer class
 **/
void writer::init(const std::string& name, const int count) {
    out.open(name);
    threadCount = count;
}

void writer::run() {
    if (out.is_open()){
        pthread_t threads[threadCount];
        int n = 0;

        while (!queue.empty()){
            if (pthread_create(threads + n++, NULL, &runner, NULL) != 0){
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

        out.close();
    }
    else {
        std::cerr << "Error: Outfile could not be opened" << std::endl;
    }
}

void* writer::runner(void* arg) { 
    out << queue.front() << std::endl;
    queue.pop_front();
    return nullptr; 
}

void writer::append(const std::string& line) {
    queue.push_back(line);
}

//void writer::setfinished() {}
