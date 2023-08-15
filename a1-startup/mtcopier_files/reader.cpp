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
pthread_mutex_t *reader::countLock;
pthread_mutex_t *reader::stillReadingLock;
pthread_t *reader::readThreads;
bool reader::stillReading;
int reader::count = 0;
int reader::finishedThreads = 0;
pthread_mutex_t *reader::finishedThreadsLock;

void reader::init(const std::string& name, const int count, writer* myWriter){
    in.open(name);
    threadCount = count;
    theWriter = myWriter;
    stillReading = true;

    lock = new pthread_mutex_t;
    if (pthread_mutex_init(lock,0) != 0){
        printf("There was an error initialising pthread mutex \n");
        exit(EXIT_FAILURE);
    }

    countLock = new pthread_mutex_t;
    if (pthread_mutex_init(countLock,0) != 0){
        printf("There was an error initialising pthread mutex \n");
        exit(EXIT_FAILURE);
    }

    stillReadingLock = new pthread_mutex_t;
    if (pthread_mutex_init(stillReadingLock,0) != 0){
        printf("There was an error initialising pthread mutex \n");
        exit(EXIT_FAILURE);
    }

    finishedThreadsLock = new pthread_mutex_t;
    if (pthread_mutex_init(finishedThreadsLock,0) != 0){
        printf("There was an error initialising pthread mutex \n");
        exit(EXIT_FAILURE);
    }
}

void reader::run() {
    if (in.is_open()) {
        readThreads = (pthread_t*)malloc(threadCount * sizeof(pthread_t));
        if (!readThreads){
            printf("There was an error with malloc \n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < threadCount; i++){
            size_t tmp = i;
            if (pthread_create(&readThreads[i], NULL, &runner, (void *)tmp) != 0){
                printf("There was an error creating thread Number %d \n", i);
                exit(EXIT_FAILURE);
            }
        }
    }
    else {
        printf("Error: Infile could not be opened \n");
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
                theWriter->append(line);
                pthread_mutex_lock(countLock);
                count++;
                pthread_cond_signal(theWriter->queueNotEmpty);
                pthread_mutex_unlock(countLock);
            }
        }
        pthread_mutex_unlock(lock);
    }


    pthread_mutex_lock(finishedThreadsLock);
    finishedThreads++;
    if (finishedThreads == threadCount){
        pthread_mutex_lock(stillReadingLock);
        stillReading = false;
        pthread_cond_broadcast(theWriter->queueNotEmpty);
        pthread_mutex_unlock(stillReadingLock);
    }
    pthread_mutex_unlock(finishedThreadsLock);

    return nullptr; 
}

void reader::join(){
    for(int i = 0; i < threadCount; i++) {
    if (pthread_join(readThreads[i], NULL) != 0){
        printf("There was an error joining thread Number %d \n", i);
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

int reader::getLineCount(){
    pthread_mutex_lock(countLock);
    int c = count;
    pthread_mutex_unlock(countLock);
    return c;
}

void reader::reduceLineCount(){
    pthread_mutex_lock(countLock);
    count--;
    pthread_mutex_unlock(countLock);
}

void reader::clean(){
    if (lock){
        pthread_mutex_destroy(lock);
        delete(lock);
    }
    if (countLock){
        pthread_mutex_destroy(countLock);
        delete(countLock);
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