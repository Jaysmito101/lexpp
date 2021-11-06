#define LEXPP_IMPLEMENTATION
#include "../lexpp.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

struct Email{
    std::string name;
    std::string domainFront;
    std::string domainEnd;
    std::string domain;
};

class EmailTokenParser : public lexpp::TokenParser
{
    public:
    EmailTokenParser(std::string data, std::string separators = "\n@.")
    :TokenParser(data, separators, true){}

    virtual int process_token(std::string& token, bool* discard, bool isSeparator, lexpp::Token* tok) override
    {
        if(isSeparator){
            if(ci == 2){
                currMail.domain = currMail.domainFront + "." + currMail.domainEnd;
                emailIds.push_back(currMail);
                ci = 0;
                *discard = true;
                return 0;  
            }
            if(token.size() <= 0){
                *discard = true;
                return 0;  
            }
            if(token == "\n"){
                ci = 0;
                *discard = true;
                return 0;  
            }
            else if(token == "@"){
                ci = 1;
                *discard = true;
                return 0;                
            }
            else if(token == "."){
                ci = 2;
                *discard = true;
                return 0;                
            }
        }

        if(ci == 0)
            currMail.name = token;
        else if(ci == 1)
            currMail.domainFront = token;
        else if(ci == 2)
            currMail.domainEnd = token;
    }    

    int ci = 0;
    Email currMail;
    std::vector<Email> emailIds;
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
    std::shared_ptr<EmailTokenParser> tok_parser = std::make_shared<EmailTokenParser>(data+"\n", "\n@.");
    lexpp::lex(tok_parser);
    for(Email& email : tok_parser->emailIds){
        std::cout << "Email : \nNAME: " << email.name << "\nDOMAIN : " << email.domain << std::endl;
    }
    
    return 0;
}