#define LEXPP_IMPLEMENTATION
#include "lexpp.h"
#include "extensions/xml_parser.h"

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

    std::shared_ptr<lexpp::XMLParser> parser = std::make_shared<lexpp::XMLParser>(data);
    lexpp::lex(parser);

    lexpp::XMLDocumentNode* root = parser->get_root_node();

    std::cout << (*root)["xml-stylesheet"]["CATALOG"]["PLANT"]["COMMON"].value << std::endl;

    std::cout << std::endl;

    std::cout << root;

    return 0;
}