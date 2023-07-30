/**
 * startup code provided by Paul Miller for COSC1114 - Operating Systems
 * Principles
 **/
#include "reader.h"
#include <functional>

reader::reader(const std::string& name, writer& mywriter) : thewriter(mywriter) {
    in.open(name);
    std::cout<<"&mywriter: "<<&mywriter <<std::endl;
    std::cout<<"&thewriter: "<<&thewriter <<std::endl;
}

void reader::run() {
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)){
            std::cout<<"std::getline line: "<<line<<std::endl;
            std::cout<<"&thewriter: "<<&thewriter <<std::endl;
            thewriter.append(line);
        }
        in.close();
    }
    else {
        std::cerr << "Error: Infile could not be opened" << std::endl;
    }
}
