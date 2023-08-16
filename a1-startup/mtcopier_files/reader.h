/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include <pthread.h>
#include "writer.h"
#include "../TimerStruct.h"
#include <fstream>
#include <iostream>
#include <string>
#ifndef READER
#define READER
class reader {
   public:
    /* this class has a bunch of static (which means shared in a class)
     * because we need to share the state between threads. For example, we
     * are reading from the same file, should have the same locks for all
     * readers, etc.
     **/

    /** initialize the shared data for the class - for example, open the
     * file. There will be other things which you will need to figure out
     * as you complete the assignment.
     **/
    static void init(const std::string& name, const int count, writer* myWriter);

    /**
     * the method that implements the thread. It has to be static as the first
     * implicit argument to all instance methods is 'this' and c functions don't
     * know about classes
     **/
    static void* runner(void*);

    /**
     * does the setup for and launches the threads
     **/
    void run();
    /**
     * there may be other functions you need so declare them.
     **/

    void join();
    static bool ifReading();
    void clean();
    static void awakenAllWriterThreads();
    static void addLine(std::string line);

   private:
    static std::ifstream in;
    static int threadCount;
    static writer* theWriter;
    static pthread_t *readThreads;
    static bool stillReading;
    static pthread_mutex_t *lock;
    static pthread_mutex_t *stillReadingLock;
    static int finishedThreads;
    static pthread_mutex_t *finishedThreadsLock;
    /**
     * There may be other private instance data you need so declare those here.
     **/
};
#endif
