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

namespace lexpp
{

    enum SyntaxTokenType
    {
        None = 0,
        String,
        Character,
        Number,
        Operator,
        Separator,
        Braces,
        Keyword,
        Identifier
    };

    enum SyntaxParserLanguage
    {
        C = 0,
        CPlusPlus,
        Java,
        Python,
        CSharp,
        Rust,
        Go,
        GLSL,
        HLSL,
        JavaScript,
        Lua
    };

    struct SyntaxToken{
        std::string value = "";
        SyntaxTokenType type = SyntaxTokenType::None;
        void* userdata = nullptr;
    };

    std::string to_string(SyntaxTokenType type);

    class SyntaxParser : public TokenParser{
        public:
        SyntaxParser(std::string data, SyntaxParserLanguage language);
        
        virtual int process_token(std::string& token, bool* discard, bool isSeparator, Token* tok) override;

        std::vector<SyntaxToken> get_tokens();

        virtual std::vector<std::string> get_operators();
        virtual std::vector<std::string> get_keywords();
        virtual std::vector<std::string> get_separators();

        private:
        bool is_number(std::string& s);
        void push_token();

        private:
        std::vector<std::string> _keywords;
        std::vector<std::string> _operators;
        SyntaxParserLanguage _language;
        SyntaxToken _currentToken;
        std::vector<SyntaxToken> _synaxTokens;
        // Flags
        bool isInString = false;
        bool isInChar = false;
    };

#ifdef LEXPP_IMPLEMENTATION

    // Function Implementations

    std::string to_string(SyntaxTokenType type)
    {
        switch(type)
        {
            case None           : return "None";  
            case String         : return "String";      
            case Character      : return "Character";      
            case Number         : return "Number";      
            case Operator       : return "Operator";      
            case Separator      : return "Separator";      
            case Braces         : return "Braces";      
            case Keyword        : return "Keyword";      
            case Identifier      : return "Identifier";      
        }
        return "";
    }

    // Class Implementations

    SyntaxParser::SyntaxParser(std::string data, SyntaxParserLanguage language = SyntaxParserLanguage::C)
    {
        _data = data;
        _language = language;
        _separators = get_separators();
        _keywords = get_keywords();
        _operators = get_operators();
        _includeSeparators = true;
    }

    std::vector<SyntaxToken> SyntaxParser::get_tokens()
    {
        return _synaxTokens;
    }

    std::vector<std::string> SyntaxParser::get_separators()
    {
        return {" ", "\n", ".", "!", "\t", ";", ":", "\\", "/", "+", "-", "*", "&", "%", "<", ">", "=", "(", ")", "{", "}", "[", "]", "\"", "\'", ","};
    }

    std::vector<std::string> SyntaxParser::get_operators()
    {
        return {"+", "-", "*", "/", "=", "<", ">", "!", "?", ":", "::", "->", "<=", ">=", "+=", "-=", "/=", "*=", "^", "^=", "&", "&&", "==", "&=", "||", "|"};
    }

    std::vector<std::string> SyntaxParser::get_keywords()
    {
        return {"for", "int"};
    }

    void SyntaxParser::push_token()
    {
        if(_currentToken.type != SyntaxTokenType::None && _currentToken.value.size() > 0)
            _synaxTokens.push_back(_currentToken);
        _currentToken = SyntaxToken();
    }

    int SyntaxParser::process_token(std::string& token, bool* discard, bool isSeparator, Token* tok)
    {
        if(isSeparator)
        {
            if(token == "." && (!isInString && !isInChar))
            {
                if(_currentToken.type == Number)
                    _currentToken.value += token;
                else
                {
                    push_token();
                    _currentToken.type = SyntaxTokenType::Operator;
                    _currentToken.value = ".";
                    push_token();
                }
            }
            else if(std::find(_operators.begin(), _operators.end(), token) != _operators.end() && (!isInString && !isInChar))
            {
                if(_currentToken.type == SyntaxTokenType::Operator)
                {
                    std::string cVal = _currentToken.value + token;
                    if(std::find(_operators.begin(), _operators.end(), cVal) != _operators.end())
                    {
                        _currentToken.value = cVal;
                        _currentToken.type = SyntaxTokenType::Operator;
                        push_token();
                    }
                    else
                    {                        
                        push_token();                    
                        _currentToken.type = SyntaxTokenType::Operator;
                        _currentToken.value = token;
                        push_token();

                    }                    
                }
                else
                {
                    push_token();
                    _currentToken.type = SyntaxTokenType::Operator;
                    _currentToken.value = token;
                }
            }
            else if( (token == "}" || token == "{" || token == "(" || token == ")" || token == "[" || token == "]") && (!isInString && !isInChar))
            {
                push_token();
                _currentToken.type = SyntaxTokenType::Braces;
                _currentToken.value = token;
                push_token();
            }
            else if(token == "\"")
            {
                if(isInString)
                {
                    _currentToken.type = SyntaxTokenType::String;
                    if(_currentToken.value[_currentToken.value.size() - 1 ] != '\\')
                        push_token();
                    isInString = false;
                }
                else
                {
                    push_token();
                    isInString = true;
                }
            }
            else if(token == "\'")
            {
                if(isInChar)
                {
                    _currentToken.type = SyntaxTokenType::Character;
                    if(_currentToken.value[_currentToken.value.size() - 1 ] != '\\')
                        push_token();
                    isInChar = false;
                }
                else
                {
                    push_token();
                    isInChar = false;
                }
            }
            else if(token == " " || token == "\n" || token == "\t"){
                if(!isInString && !isInChar)
                    push_token();
            }
            else if(isInChar || isInString)
                _currentToken.value += token;
        }
        else
        {
            if(isInChar)
                _currentToken.type = SyntaxTokenType::Character;
            else if(isInString)
                _currentToken.type = SyntaxTokenType::String;
            else 
            {
                push_token();
                if(is_number(token))
                    _currentToken.type = SyntaxTokenType::Number;
                else if(std::find(_keywords.begin(), _keywords.end(), token) != _keywords.end())
                    _currentToken.type = SyntaxTokenType::Keyword;
                else
                    _currentToken.type = SyntaxTokenType::Identifier;
            }
            _currentToken.value += token;
        }
        *discard = true;
        return 0;
    }

    bool SyntaxParser::is_number(std::string& s)
    {
        return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return (!std::isdigit(c) && c != '.' && c!='f'); }) == s.end();
    }

#endif
    
}

#endif