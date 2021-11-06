#define LEXPP_IMPLEMENTATION
#include "../lexpp.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

enum MyTokens{
    Keyword = 0,
    Number,
    String,
    Other
};

static std::string TokenToString(int tok){
    switch(tok){
        case Keyword: return "Keyword";
        case Number:  return "Number";
        case String:  return "String";
        case Other:   return "Other";
    }
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}


class MyTokenParser : public lexpp::TokenParser
{
    public:
    MyTokenParser(std::string data, std::string separators)
    :TokenParser(data, separators, false){}

    virtual int process_token(std::string& token, bool* discard, bool isSeparator, lexpp::Token* tok) override
    {
        if(std::find(keywords.begin(), keywords.end(), token) != keywords.end())
            return MyTokens::Keyword;
        else if(is_number(token))
            return MyTokens::Number;
        else if(isSeparator)
            return MyTokens::Other;
        else
            return MyTokens::String;
    }    

    std::vector<std::string> keywords = {"for", "void", "return", "if", "int"};
};

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
    std::vector<lexpp::Token> tokens = lexpp::lex(std::make_shared<MyTokenParser>(data, "\n :,[]{}().\t"));
    for(lexpp::Token& token : tokens){
        std::cout << TokenToString(token.type) << " -> " << token.value << std::endl;
    }
    
    return 0;
}