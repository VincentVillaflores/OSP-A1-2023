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
    std::cout<<"writer.run() queue: "<<queue[0]<<std::endl;
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
    std::cout<<"append()1 queue: "<<queue[0]<<std::endl;
    queue.push_back(line);
    std::cout<<"append()2 queue: "<<queue[0]<<std::endl;
}
