/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"

/**
 * provide your implementation for the writer functions here
 **/
writer::writer(const std::string& name) {
    out.open(name);
}

void writer::run(TimerStruct* timer) {
    if (out.is_open()){
        for (std::string line : queue){
            clock_t start = clock();
            out << line << std::endl;
            clock_t end = clock();
            clock_t dur = end - start;
            timer->writeLineDuration.push_back(dur);
        }
        out.close();
    }
    else {
        std::cerr << "Error: Outfile could not be opened" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void writer::append(const std::string& line) {
    queue.push_back(line);
}
