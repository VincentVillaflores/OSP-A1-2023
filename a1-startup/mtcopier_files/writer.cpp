/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "writer.h"

#include "reader.h"

/**
 * implement these functions requred for the writer class
 **/
int writer::threadCount = 0;
std::ofstream writer::out;
std::deque<std::string> writer::queue;

void writer::init(const std::string& name, const int count) {
    out.open(name);
    threadCount = count;
}

void writer::run() {
    if (out.is_open()){
        pthread_t threads[threadCount];

        for(int i = 0; i < threadCount; i++) {
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

        out.close();
    }
    else {
        std::cerr << "Error: Outfile could not be opened" << std::endl;
    }
}

void* writer::runner(void* arg) { 
    while (!queue.empty()){
        std::cout<<"Thread: "<<(size_t) arg<<" Line: "<<queue.front()<<std::endl;
        out << queue.front() << std::endl;
        queue.pop_front();
    }
    return nullptr; 
}

void writer::append(const std::string& line) {
        queue.push_back(line);
}

//void writer::setfinished() {}
