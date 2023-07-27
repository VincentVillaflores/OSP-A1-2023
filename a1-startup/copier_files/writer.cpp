/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/

#include "writer.h"

/**
 * provide your implementation for the writer functions here
 **/
writer::writer(const std::string& name) {
    std::ofstream out(name);
}

void writer::run() {}

void writer::append(const std::string& line) {
    queue.push_back(line);
}
