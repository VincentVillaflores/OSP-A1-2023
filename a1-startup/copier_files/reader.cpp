/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>

reader::reader(const std::string& name, writer& mywriter) : reader(name, thewriter) {
    std::ifstream in(name);
}

void reader::run() {
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)){
            thewriter.append(line);
        }
        in.close();
    }
}
