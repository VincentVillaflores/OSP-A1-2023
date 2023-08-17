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
pthread_t *writer::writeThreads;
pthread_cond_t *writer::queueNotEmpty;
pthread_mutex_t *writer::lock;
pthread_mutex_t *writer::queueLock;
TimerStruct *writer::theTimer;

void writer::init(const std::string& name, const int count, TimerStruct* timer) {
    out.open(name);
    threadCount = count;
    theTimer = timer;

    queueNotEmpty = new pthread_cond_t;
    if (pthread_cond_init(queueNotEmpty,0) != 0){
        std::cerr<<"There was an error initialising pthread cond"<<std::endl;
        exit(EXIT_FAILURE);
    }

    lock = new pthread_mutex_t ;
    if (pthread_mutex_init(lock,0) != 0){
        std::cerr<<"There was an error initialising pthread mutex \n"<<std::endl;
        exit(EXIT_FAILURE);
    }

    queueLock = new pthread_mutex_t;
    if (pthread_mutex_init(queueLock,0) != 0){
        std::cerr<<"There was an error initialising pthread mutex"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

void writer::run() {
    writeThreads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
    if (!writeThreads){
        std::cerr<<"There was an error with malloc"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if (out.is_open()){
        for(int i = 0; i < threadCount; i++) {
            size_t tmp = i;
            if (pthread_create(&writeThreads[i], NULL, &runner, (void*) tmp) != 0){
                std::cerr<<"There was an error creating thread Number "<<i<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        std::cerr<<"Error: Outfile could not be opened"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

void* writer::runner(void* arg) {
    while(reader::ifReading() || getQueueCount() != 0){
        pthread_mutex_lock(lock);

        while(getQueueCount() == 0 && reader::ifReading()){
            pthread_cond_wait(queueNotEmpty, lock);
        }

        if (getQueueCount() != 0){
            theTimer->writeLineStart.push_back(clock());
            writeFrontLineInQueue();
            theTimer->writeLineEnd.push_back(clock());
        }

        pthread_mutex_unlock(lock);
    }
    return nullptr; 
}

int writer::getQueueCount(){
    pthread_mutex_lock(queueLock);
    int c = queue.size();
    pthread_mutex_unlock(queueLock);
    return c;
}

void writer::writeFrontLineInQueue(){
    pthread_mutex_lock(queueLock);
    out << queue.front() << std::endl;
    queue.pop_front();
    pthread_mutex_unlock(queueLock);
}

void writer::append(const std::string& line) {
    pthread_mutex_lock(queueLock);
    queue.push_back(line);
    pthread_mutex_unlock(queueLock);
}

void writer::join(){
    for(int i = 0; i < threadCount; i++) {
        if (pthread_join(writeThreads[i], NULL) != 0){
            std::cerr<<"There was an error joining thread Number "<<i<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    free(writeThreads);
    out.close();
}

void writer::clean(){
    if (lock){
        pthread_mutex_destroy(lock);
        delete(lock);
    }

    if (queueLock){
        pthread_mutex_destroy(queueLock);
        delete(queueLock);
    }

    if (queueNotEmpty){
        pthread_cond_destroy(queueNotEmpty);
        delete(queueNotEmpty);
    }
}
