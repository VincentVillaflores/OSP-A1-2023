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
    if (argc < 3) {
        std::cout<<"Please pass an input and output file!"<<std::endl;
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
