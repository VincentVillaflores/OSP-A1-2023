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
    bool time = false;
    if (argc != 3 && argc != 4) {
        std::cout<<"Please pass an input and output file!"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string str = "-t";
    time = argv[3] == str;
    if (argc == 4 && !time){
        std::cerr<<"The only valid optional flag is -t!"<<std::endl;
        return EXIT_FAILURE;
    }
    else {
        writer write(argv[2]);
        reader read(argv[1], write);
        read.run();
        write.run();
    }
    
    /* load the file and copy it to the destination */
    return EXIT_SUCCESS;
}
