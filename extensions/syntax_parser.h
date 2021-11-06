/*
MIT License

Copyright (c) 2021 Jaysmito Mukherjee (jaysmito101@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef LEXPP_SYNTAX_PARSER_H
#define LEXPP_SYNTAX_PARSER_H

#include "../lexpp.h"

namespace lexpp{

    enum SyntaxTokenType{
        None = 0,
        String,
        Number,
        Operator,
        Separator,
        Braces,
        Keyword,
        Identifier
    };

    struct SyntaxToken{
        std::string value;
        SyntaxTokenType type;
        void* userdata;
    };

    class SyntaxParser : public TokenParser{
        public:
        SyntaxParser(std::string data, std::vector<std::string> separators);
        
        virtual int process_token(std::string& token, bool* discard, bool isSeparator, Token* tok) = 0;

        std::vector<SyntaxToken> get_tokens();

        private:
        SyntaxToken currentToken;
        std::vector<SyntaxToken> synaxTokens;
    };

#ifdef LEXPP_IMPLEMENTATION

    SyntaxParser::SyntaxParser(std::string data, std::vector<std::string> separators)
    :TokenParser(data, separators, true)
    {}

    std::vector<SyntaxToken> SyntaxParser::get_tokens()
    {
        return synaxTokens;
    }

#endif
    

#ifdef LEXPP_SYNTAX_PARSER_C

    class SyntaxParserC : public SyntaxParser{
        public:
        SyntaxParserC(std::string data);

        virtual int process_token(std::string& token, bool* discard, bool isSeparator, Token* tok) override;

        private:
        std::vector<std::string> _keywordsC = {"for", "int"};
        std::vector<std::string> _separatorsC = {" ", "\n", ".", "!", "\t", ";", ":", "\\", "/", "+", "-", "*", "&", "%", "<", ">", "=", "(", ")", "{", "}", "[", "]"};
    };

#ifdef LEXPP_IMPLEMENTATION

    SyntaxParserC::SyntaxParserC(std::string data)
    :SyntaxParser(data, _separatorsC)
    {

    }

    int SyntaxParserC::process_token(std::string& token, bool* discard, bool isSeparator, Token* tok){return 0;}

#endif

#endif

}

#endif