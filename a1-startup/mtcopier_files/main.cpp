/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include <cstdlib>
#include <unistd.h>
/**
 * these need to be global variables as you'll need handle cleaning them up in
 *cleanup which will automatically be called on program exit
 **/
reader* readers;
writer* writers;
TimerStruct* timer;
clock_t programStart;
bool willTime;

void cleanup() {
    /**
     * perform any cleanup you need to do here for global pointers
     **/
    if (willTime){
        clock_t programEnd = clock();
        clock_t programDuration = programEnd - programStart;
        long programDurationInSeconds = (long) programDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Program Duration: "<<programDurationInSeconds<<" seconds." <<std::endl;
        clock_t totalReadDuration = 0;
        clock_t totalWriteDuration = 0;
        clock_t totalReadWaitDuration = 0;
        std::size_t totalLines = timer->readLineDuration.size();
        for (std::size_t i = 0; i < totalLines; i++){
            totalReadDuration += timer->readLineDuration[i];
            totalWriteDuration += timer->writeLineDuration[i];
            totalReadWaitDuration += timer->readLineWaitDuration[i];
        }

        long readDurationInSeconds = (long) totalReadDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Read Duration: "<<readDurationInSeconds<<" seconds." <<std::endl;
        long averageReadDurationInSeconds = readDurationInSeconds / totalLines;
        std::cout<<"Average Read Duration: "<<averageReadDurationInSeconds<<" seconds." <<std::endl;

        long readWaitDurationInSeconds = (long) totalReadWaitDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Read Wait Duration: "<<readWaitDurationInSeconds<<" seconds." <<std::endl;
        long averageReadWaitDurationInSeconds = readWaitDurationInSeconds / totalLines;
        std::cout<<"Average Read Wait Duration: "<<averageReadWaitDurationInSeconds<<" seconds." <<std::endl;

        long writeDurationInSeconds = (long) totalWriteDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Write Duration: "<<writeDurationInSeconds<<" seconds." <<std::endl;
        long averageWriteDurationInSeconds = writeDurationInSeconds / totalLines;
        std::cout<<"Average Write Duration: "<<averageWriteDurationInSeconds<<" seconds." <<std::endl;

        long totalOverheadDuration = programDurationInSeconds - readDurationInSeconds - writeDurationInSeconds;
        std::cout<<"Total Overhead Duration: "<<totalOverheadDuration<<" seconds." <<std::endl;
    }

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
    atexit(cleanup);
    programStart = clock();
    timer = new TimerStruct();
    willTime = false;
    /**
     * check command line arguments
     **/
    int c = atoi(argv[1]);
    if (argc != 4 && argc != 5) {
        std::cerr<<"Please pass a thread count, an input and output file! \n"<<std::endl;
        return EXIT_FAILURE;
        
    }
    if (argc == 5){
        std::string str = "-t";
        willTime= argv[4] == str;
        if (!willTime){
            std::cerr<<"The only valid optional flag is -t!"<<std::endl;
            return EXIT_FAILURE;
        }
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
    
    /**
     *
     * have loop here waiting for the threads to bomplete. Please see
     * section on avoiding busy waiting on the assignment specification to see
     * what need to be done here
      **/
    return EXIT_SUCCESS;
}
