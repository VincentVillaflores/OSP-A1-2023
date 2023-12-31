/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>

reader::reader(const std::string& name, writer& mywriter) : thewriter(mywriter) {
    in.open(name);
}

void reader::run(TimerStruct* timer) {
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)){
            clock_t start = clock();
            thewriter.append(line);
            clock_t end = clock();
            clock_t dur = end - start;
            timer->readLineDuration.push_back(dur);
        }
        in.close();
    }
    else {
        std::cerr << "Error: Infile could not be opened" << std::endl;
        exit(EXIT_FAILURE);
    }
}
