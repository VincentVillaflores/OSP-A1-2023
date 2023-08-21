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
writer *reader::theWriter;
pthread_mutex_t *reader::lock;
pthread_mutex_t *reader::stillReadingLock;
pthread_t *reader::readThreads;
bool reader::stillReading;
int reader::finishedThreads = 0;
pthread_mutex_t *reader::finishedThreadsLock;
TimerStruct *reader::thisTimer;

void reader::init(const std::string& name, const int count, writer* myWriter, TimerStruct* timer){
    in.open(name);
    threadCount = count;
    theWriter = myWriter;
    stillReading = true;
    thisTimer = timer;

    lock = new pthread_mutex_t;
    if (pthread_mutex_init(lock,0) != 0){
        std::cerr<<"There was an error initialising pthread mutex"<<std::endl;
        exit(EXIT_FAILURE);
    }

    stillReadingLock = new pthread_mutex_t;
    if (pthread_mutex_init(stillReadingLock,0) != 0){
        std::cerr<<"There was an error initialising pthread mutex"<<std::endl;
        exit(EXIT_FAILURE);
    }

    finishedThreadsLock = new pthread_mutex_t;
    if (pthread_mutex_init(finishedThreadsLock,0) != 0){
        std::cerr<<"There was an error initialising pthread mutex"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

void reader::run() {
    if (in.is_open()) {
        readThreads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
        if (!readThreads){
            std::cerr<<"There was an error with malloc"<<std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < threadCount; i++){
            size_t tmp = i;
            if (pthread_create(&readThreads[i], NULL, &runner, (void *)tmp) != 0){
                std::cerr<<"There was an error creating thread Number "<<i<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        std::cerr<<"Error: Infile could not be opened"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

void* reader::runner(void* arg) {
    while (in.good()){ 
        std::string line;
        pthread_mutex_lock(lock);
        if (in.good()){
            getline(in,line);

            if (!line.empty()){
                addLine(line);
            }
        }
        pthread_mutex_unlock(lock);
    }

    awakenAllWriterThreads();

    return nullptr; 
}

//adds line to writer queue
//awakens one writer thread
void reader::addLine(std::string line){
    clock_t start = clock();
    theWriter->append(line);
    clock_t end = clock();
    clock_t dur = end - start;
    thisTimer->readLineWaitDuration.push_back(dur);
    pthread_cond_signal(theWriter->queueNotEmpty);
}

//counts the number of reader threads that have finished
//last thread will wake up all waiting writer threads
void reader::awakenAllWriterThreads(){
    pthread_mutex_lock(finishedThreadsLock);
    finishedThreads++;
    if (finishedThreads == threadCount){
        pthread_mutex_lock(stillReadingLock);
        stillReading = false;
        pthread_cond_broadcast(theWriter->queueNotEmpty);
        pthread_mutex_unlock(stillReadingLock);
    }
    pthread_mutex_unlock(finishedThreadsLock);
}

void reader::join(){
    for(int i = 0; i < threadCount; i++) {
    if (pthread_join(readThreads[i], NULL) != 0){
        std::cerr<<"There was an error joining thread Number "<<i<<std::endl;
        exit(EXIT_FAILURE);
        }   
    }
    in.close();
    free(readThreads);
}

bool reader::ifReading(){
    pthread_mutex_lock(stillReadingLock);
    bool b = stillReading;
    pthread_mutex_unlock(stillReadingLock);

    return b;
}

void reader::clean(){
    if (lock){
        pthread_mutex_destroy(lock);
        delete(lock);
    }
    if (stillReadingLock){
        pthread_mutex_destroy(stillReadingLock);
        delete(stillReadingLock);
    }
    if (finishedThreadsLock){
        pthread_mutex_destroy(finishedThreadsLock);
        delete(finishedThreadsLock);
    }
}