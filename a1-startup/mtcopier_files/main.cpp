/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include <cstdlib>
/**
 * these need to be global variables as you'll need handle cleaning them up in
 *cleanup which will automatically be called on program exit
 **/
reader* readers;
writer* writers;

void cleanup() {
    /**
     * perform any cleanup you need to do here for global pointers
     **/
}

int main(int argc, char** argv) {
    atexit(cleanup);
    /**
     * check command line arguments
     **/
    int c = atoi(argv[1]); //must be between 25-100 check later
    if (argc < 3) {
        std::cout<<"Please pass an input and output file!"<<std::endl;
    }
    /**
     * process command line arguments
     **/
    
    else {
        /**
         * initiliaze the reader and writer classes
         **/
        writer.init(argv[2], c);
        reader.init(argv[2], c, writers);
        /**
         * initialize the running of each thread. Note you just call run() on each
         * object here, you'll call pthread_create itself in the run function.
         **/
        readers->run();
        writers->run();
    }
    /**
     *
     * have loop here waiting for the threads to bomplete. Please see
     * section on avoiding busy waiting on the assignment specification to see
     * what need to be done here
      **/
    return EXIT_SUCCESS;
}
