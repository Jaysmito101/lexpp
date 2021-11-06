#define LEXPP_IMPLEMENTATION
#include "../lexpp.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

int main(int argc, char** argv){

    if(argc <= 1){
        std::cout << "Usage : lexpp filename" << std::endl;
        exit(-1);
    }
    std::string filename = std::string(argv[1]);
    std::ifstream t(filename.c_str());
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string data(size, ' ');
    t.seekg(0);
    t.read(&data[0], size);

    std::vector<std::string> tokens = lexpp::lex(data, {"<=", "<<", "\n", "::", ",", "}", "{", ";", " "}, false);

    for(std::string& token : tokens){
        std::cout << token << std::endl;
    }
    
    return 0;
}