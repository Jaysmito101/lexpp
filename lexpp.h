#ifndef LEXPP_H
#define LEXPP_H

// Includes
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>

namespace lexpp{

    class TokenParser
    {
        public:
        TokenParser(std::string data, std::string separators = " \n", bool includeSeparators = false);
        TokenParser(std::string data, std::vector<std::string> separators = {" ", "\n"}, bool includeSeparators = false);
        std::vector<std::string> get_separators();
        std::string get_data();
        bool include_separators();

        virtual int process_token(std::string& token, bool* discard, bool isSeparator) = 0;
    
        private:
        std::string _data;
        std::vector<std::string> _separators;
        bool _includeSeparators;
    };

    struct Token
    {
        std::string value;
        int type;
    };

    // To check is a string ends with another string
    bool ends_with(std::string& value, std::string& ending);

    // Docs comming soon ...
    std::vector<std::string> lex(std::string data, std::string separators, bool includeSeparators = false);
    
    // Docs comming soon ...
    std::vector<std::string> lex(std::string data, std::vector<std::string> separators, bool includeSeparators = false);
    
    // Docs comming soon ...
    std::vector<Token> lex(std::string data, std::vector<std::string> separators, std::function<int(std::string&, bool*, bool)> tokenFunction, bool includeSeparators = false);

    // Docs comming soon ...
    std::vector<Token> lex(std::shared_ptr<TokenParser> parser);

#ifdef LEXPP_IMPLEMENTATION

// Functions implementations

    bool ends_with(std::string& value, std::string& ending)
    {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    std::vector<std::string> lex(std::string data, std::string separators, bool includeSeparators)
    {
        // Store individual tokens
        std::string token;
        // The tokens for return
        std::vector<std::string> tokens;
        for(int i = 0 ; i < data.size() ; i++){
            // If the character is a separator then push the token before it to tokens list
            if(separators.find(data[i]) != std::string::npos){
                // Only push token if its length > 0
                if(token.size() > 0)
                    tokens.push_back(token);
                if(includeSeparators)
                    tokens.push_back(std::string(1, data[i]));
                token = "";
            }
            else{
                token += data[i];
            }
        }
        if(token.size() > 0)
            tokens.push_back(token);
        return tokens;
    }

    std::vector<std::string> lex(std::string data, std::vector<std::string> separators, bool includeSeparators)
    {
        // Store individual tokens
        std::string token;
        // The tokens for return
        std::vector<std::string> tokens;
        for(int i = 0 ; i < data.size() ; i++){
            // If the token string ends with a separator then push the token before it to tokens list

            std::string sep = "";
            for(std::string& separator : separators){
                if(ends_with(token, separator)){
                    sep = separator;
                    break;
                }
            }
            if(sep != ""){
                // Only push token if its length > 0
                token = token.substr(0, token.size() - sep.size());
                if(token.size() > 0)
                    tokens.push_back(token);
                if(includeSeparators)
                    tokens.push_back(sep);
                token = "";
            }
            token += data[i];
        }
        if(token.size() > 0)
            tokens.push_back(token);
        return tokens;
    }

    std::vector<Token> lex(std::string data, std::vector<std::string> separators, std::function<int(std::string&, bool*, bool)> tokenFunction, bool includeSeparators)
    {
           // Store individual tokens
        std::string token;
        // The tokens for return
        std::vector<Token> tokens;
        for(int i = 0 ; i < data.size() ; i++){
            // If the token string ends with a separator then push the token before it to tokens list

            std::string sep = "";
            for(std::string& separator : separators){
                if(ends_with(token, separator)){
                    sep = separator;
                    break;
                }
            }
            if(sep != ""){
                // Only push token if its length > 0
                token = token.substr(0, token.size() - sep.size());
                if(token.size() > 0){
                    Token tok;
                    bool discard = false;
                    tok.type = tokenFunction(token, &discard, false);
                    tok.value = token;
                    if(!discard)
                        tokens.push_back(tok);
                }
                if(includeSeparators){
                    Token tok;
                    bool discard = false;
                    tok.type = tokenFunction(sep, &discard, true);
                    tok.value = sep;
                    if(!discard)
                        tokens.push_back(tok);
                }
                token = "";
            }
            token += data[i];
        }
        Token tok;
        bool discard = false;
        tok.type = tokenFunction(token, &discard, false);
        tok.value = token;
        if(!discard)
            tokens.push_back(tok);
        return tokens;
    }

    std::vector<Token> lex(std::shared_ptr<TokenParser> parser)
    {
        std::string data = parser->get_data();
        std::vector<std::string> separators = parser->get_separators();
        bool includeSeparators = parser->include_separators();
        // Store individual tokens
        std::string token;
        // The tokens for return
        std::vector<Token> tokens;
        for(int i = 0 ; i < data.size() ; i++){
            // If the token string ends with a separator then push the token before it to tokens list

            std::string sep = "";
            for(std::string& separator : separators){
                if(ends_with(token, separator)){
                    sep = separator;
                    break;
                }
            }
            if(sep != ""){
                // Only push token if its length > 0
                token = token.substr(0, token.size() - sep.size());
                if(token.size() > 0){
                    Token tok;
                    bool discard = false;
                    tok.type = parser->process_token(token, &discard, false);
                    tok.value = token;
                    if(!discard)
                        tokens.push_back(tok);
                }
                if(includeSeparators){
                    Token tok;
                    bool discard = false;
                    tok.type = parser->process_token(sep, &discard, true);
                    tok.value = sep;
                    if(!discard)
                        tokens.push_back(tok);
                }
                token = "";
            }
            token += data[i];
        }
        Token tok;
        bool discard = false;
        tok.type = parser->process_token(token, &discard, false);
        tok.value = token;
        if(!discard)
            tokens.push_back(tok);
        return tokens;
    }

    // Class Function Implementations

    // TokenParser

    std::string TokenParser::get_data()
    {
        return _data;
    }

    std::vector<std::string> TokenParser::get_separators()
    {
        return _separators;
    }

    bool TokenParser::include_separators()
    {
        return _includeSeparators;
    }

    TokenParser::TokenParser(std::string data, std::string separators, bool includeSeparators)
    :_data(data), _includeSeparators(includeSeparators)
    {
        for(int i=0;i<separators.size();i++){
            _separators.push_back(std::string(1, separators[i]));
        }
    }

    TokenParser::TokenParser(std::string data, std::vector<std::string> separators, bool includeSeparators)
    :_separators(separators), _data(data), _includeSeparators(includeSeparators)
    {}

#endif

}

#endif