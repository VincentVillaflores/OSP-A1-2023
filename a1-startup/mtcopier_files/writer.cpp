/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "writer.h"

#include "reader.h"

/**
 * implement these functions requred for the writer class
 **/
void writer::init(const std::string& name, const int count) {
    out.open(name);
    threadCount = count;
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

void* writer::runner(void* arg) { 
    
    return nullptr; 
    }

void writer::append(const std::string& line) {
    queue.push_back(line);
}

//void writer::setfinished() {}
