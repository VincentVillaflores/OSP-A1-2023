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
pthread_mutex_t writer::lock;
pthread_mutex_t writer::lock2;

void writer::init(const std::string& name, const int count) {
    std::cout<<"writer::innit()"<<std::endl;
    out.open(name);
    threadCount = count;
}

void writer::run() {
    std::cout<<"writer::run()"<<std::endl;
    pthread_mutex_init(&lock,0);
    pthread_mutex_init(&lock2,0);
    if (out.is_open()){
        pthread_t threads[threadCount];

        for(int i = 0; i < threadCount; i++) {
            size_t tmp = i;
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

        out.close();
        pthread_mutex_destroy(&lock);
        pthread_mutex_destroy(&lock2);
    }
    else {
        std::cerr << "Error: Outfile could not be opened" << std::endl;
    }
}

void* writer::runner(void* arg) { 
    std::cout<<"writer::runner()"<<std::endl;
    while (!queue.empty()){
        pthread_mutex_lock(&lock);
        std::cout<<"WriterThread: "<<(size_t) arg<<" Line: "<<queue.front()<<std::endl;
        out << queue.front() << std::endl;
        queue.pop_front();
        pthread_mutex_unlock(&lock);
    }
    return nullptr; 
}

void writer::append(const std::string& line) {
    pthread_mutex_lock(&lock2);
    std::cout<<"writer::append(): "<<line<<std::endl;
    queue.push_back(line);
    pthread_mutex_unlock(&lock2);
}

//void writer::setfinished() {}
