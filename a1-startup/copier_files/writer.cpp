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

void writer::run() {
    if (out.is_open()){
        for (std::string line : queue){
            out << line << std::endl;
        }
        out.close();
    }
    else {
        std::cerr << "Error: Outfile could not be opened" << std::endl;
    }
}

void writer::append(const std::string& line) {
    queue.push_back(line);
}
