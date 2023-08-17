/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include "../TimerStruct.h"
#include <cstdlib>
/**
 * these need to be global variables as you'll need handle cleaning them up in
 *cleanup which will automatically be called on program exit
 **/
reader* readers;
writer* writers;
TimerStruct* timer;

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

    if (timer){
        delete timer;
        timer = nullptr;
    }
}

int main(int argc, char** argv) {
    clock_t programStart = clock();
    timer = new TimerStruct();
    bool time = false;
    atexit(cleanup);
    /**
     * check command line arguments
     **/
    int c = atoi(argv[1]);
    if (argc != 4 && argc != 5) {
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
    if (argc == 5 && !time){
        std::string str = "-t";
        time = argv[4] == str;
        if (!time){
            std::cerr<<"The only valid optional flag is -t!"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    
    /**
     * initiliaze the reader and writer classes
     **/
    writers = new writer;
    readers = new reader;

    writers->init(argv[3], c, timer);
    readers->init(argv[2], c, writers, timer);
    /**
     * initialize the running of each thread. Note you just call run() on each
     * object here, you'll call pthread_create itself in the run function.
     **/
    writers->run();
    readers->run();

    writers->join();
    readers->join();

    clock_t programEnd = clock();
    if (time){
        clock_t programDuration = programEnd - programStart;
        int programDurationInSeconds = (int) programDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Program Duration: "<<programDurationInSeconds<<" seconds." <<std::endl;
        clock_t totalReadDuration = 0;
        clock_t totalWriteDuration = 0;
        std::size_t totalLines = timer->readLineStart.size();
        for (std::size_t i = 0; i < totalLines; i++){
            clock_t readDur = timer->readLineEnd[i] - timer->readLineStart[i];
            totalReadDuration += readDur;
            clock_t writeDur = timer->writeLineEnd[i] - timer->writeLineStart[i];
            totalWriteDuration += writeDur;
        }

        int readDurationInSeconds = (int) totalReadDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Read Duration: "<<readDurationInSeconds<<" seconds." <<std::endl;
        int averageReadDurationInSeconds = readDurationInSeconds / totalLines;
        std::cout<<"Average Read Duration: "<<averageReadDurationInSeconds<<" seconds." <<std::endl;

        int writeDurationInSeconds = (int) totalWriteDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Write Duration: "<<writeDurationInSeconds<<" seconds." <<std::endl;
        int averageWriteDurationInSeconds = writeDurationInSeconds / totalLines;
        std::cout<<"Average Write Duration: "<<averageWriteDurationInSeconds<<" seconds." <<std::endl;

        int totalOverheadDuration = programDurationInSeconds - readDurationInSeconds - writeDurationInSeconds;
        std::cout<<"Total Overhead Duration: "<<totalOverheadDuration<<" seconds." <<std::endl;
    }
    
    /**
     *
     * have loop here waiting for the threads to bomplete. Please see
     * section on avoiding busy waiting on the assignment specification to see
     * what need to be done here
      **/
    return EXIT_SUCCESS;
}
