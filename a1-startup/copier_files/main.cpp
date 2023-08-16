/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include "writer.h"
#include "../TimerStruct.h"
#include <cstdlib>
/* global variables if needed go here */
int main(int argc, char** argv) {
    /* check command line arguments */
    clock_t programStart = clock();
    TimerStruct* timer = new TimerStruct();
    bool time = false;
    if (argc != 3 && argc != 4) {
        std::cout<<"Please pass an input and output file!"<<std::endl;
        return EXIT_FAILURE;
    }
    if (argc == 4){
        std::string str = "-t";
        time = argv[3] == str;
        if (!time){
            std::cerr<<"The only valid optional flag is -t!"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    writer write(argv[2]);
    reader read(argv[1], write);
    read.run(timer);
    write.run(timer);
    
    clock_t programEnd = clock();
    if (time){
        clock_t programDuration = programEnd - programStart;
        int programDurationInSeconds = (int) programDuration / (double) CLOCKS_PER_SEC;
        std::cout<<"Total Program Duration: "<<programDurationInSeconds<<" seconds." <<std::endl;
        clock_t totalReadDuration = 0;
        clock_t totalWriteDuration = 0;
        int totalLines = timer->readLineStart.size();
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
    delete(timer);
    timer = nullptr;
    /* load the file and copy it to the destination */
    return EXIT_SUCCESS;
}
