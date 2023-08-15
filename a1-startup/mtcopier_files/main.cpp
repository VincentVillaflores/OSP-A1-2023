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

    if (readers){
        readers->clean();
        delete readers;
        readers = nullptr;
    }

    if (writers){
        writers->clean();
        delete writers;
        writers = nullptr;
    }
}

int main(int argc, char** argv) {
    atexit(cleanup);
    /**
     * check command line arguments
     **/
    int c = atoi(argv[1]); 
    if (argc != 4) {
        std::cerr<<"Please pass a thread count, an input and output file! \n"<<std::endl;
        return EXIT_FAILURE;
        
    }
    if (25 > c || c > 100) {
        std::cerr<<"Thread count must be between 25 and 100! \n"<<std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile) {
        std::cerr<<"Error: Input file not found or cannot be opened."<<std::endl;
        return EXIT_FAILURE;
    }
    inputFile.close();

    std::ofstream outputFile(argv[3]);
    if (!outputFile) {
        std::cerr<<"Error: Output file not found or cannot be opened."<<std::endl;
        return EXIT_FAILURE;
    }
    outputFile.close();
    /**
     * process command line arguments
     **/
    
    
    /**
     * initiliaze the reader and writer classes
     **/
    writers = new writer;
    readers = new reader;

    writers->init(argv[3], c);
    readers->init(argv[2], c, writers);
    /**
     * initialize the running of each thread. Note you just call run() on each
     * object here, you'll call pthread_create itself in the run function.
     **/
    writers->run();
    readers->run();

    writers->join();
    readers->join();
    
    /**
     *
     * have loop here waiting for the threads to bomplete. Please see
     * section on avoiding busy waiting on the assignment specification to see
     * what need to be done here
      **/
    return EXIT_SUCCESS;
}
