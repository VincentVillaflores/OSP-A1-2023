/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include"../TimerStruct.h"
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#ifndef WRITER
#define WRITER
class writer {
   public:
    /**
     * Please note that methods and data need to be static as there's a
     * variety of information that needs to be coordinated between writers
     * such as locks and other shared data. Also, as the pthread library
     * is a c library, it does not know about classes so ruuner in particular
     * needs to be static. You can pass in instances into the function as
     * pointers though.
     **/
    static void init(const std::string& name, const int count, TimerStruct* timer);
    static void* runner(void*);
    void run();
    static void append(const std::string& line);
    void join();
    static pthread_cond_t *queueNotEmpty;
    void clean();
    static void writeFrontLineInQueue();
    static int getQueueCount();

   private:
    static std::ofstream out;
    static std::deque<std::string> queue;
    static int threadCount;
    static pthread_t *writeThreads;
    static pthread_mutex_t *lock;
    static pthread_mutex_t *queueLock;
    static TimerStruct *thisTimer;
};
#endif
