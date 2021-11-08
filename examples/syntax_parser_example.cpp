#define LEXPP_IMPLEMENTATION
#include "lexpp.h"
#include "extensions/syntax_parser.h"

#include <iostream>
#include <iomanip>
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

    std::shared_ptr<lexpp::SyntaxParser> parser = std::make_shared<lexpp::SyntaxParser>(data);
    std::vector<lexpp::Token> tokens = lexpp::lex(parser);

    std::cout << std::setfill(' ') << std::left;

    std::cout << std::setw(30) << "Token" << std::setw(30) << "Type" << std::endl;

    for(lexpp::SyntaxToken& token : parser->get_tokens()){
        std::cout << std::setw(30) << token.value << std::setw(30) << lexpp::to_string(token.type) << std::endl;
    }    
    return 0;
}