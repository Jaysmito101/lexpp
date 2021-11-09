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
        Lua,
        CoffeeScript,
        TypeScript,
        Swift,
        Kotlin,
        ObjectiveC,
        ObjectiveCPlusPlus,
        ActionScript,
        Scala
    };

    struct SyntaxToken{
        std::string value = "";
        SyntaxTokenType type = SyntaxTokenType::None;
        void* userdata = nullptr;
    };

    std::string to_string(SyntaxTokenType type);

    std::string to_string(SyntaxParserLanguage language);

    std::ostream& operator<<(std::ostream& os, SyntaxTokenType& tok);

    std::ostream& operator<<(std::ostream& os, SyntaxParserLanguage& tok);

    class SyntaxParser : public TokenParser{
        public:
        SyntaxParser(std::string data, SyntaxParserLanguage language);
        
        virtual int process_token(std::string& token, bool* discard, bool isSeparator, Token* tok) override;

        virtual bool accept_token();
        virtual void on_token(std::string& token, bool* discard, bool isSeparator, Token* tok);

        std::vector<SyntaxToken> get_tokens();

        virtual std::vector<std::string> get_operators();
        virtual std::vector<std::string> get_keywords();
        virtual std::vector<std::string> get_separators();

        private:
        bool is_number(std::string& s);
        void push_token();

        protected:
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
            case Identifier     : return "Identifier";      
            default             : return "Unknown";
        }
        return "";
    }

    std::string to_string(SyntaxParserLanguage language)
    {
        switch(language)
        {
            case C               : return "C";
            case CPlusPlus       : return "C++";
            case Java            : return "Java";
            case Python          : return "Python";
            case CSharp          : return "C#";
            case Rust            : return "Rust";
            case Go              : return "Go";
            case GLSL            : return "GLSL";
            case HLSL            : return "HLSL";
            case JavaScript      : return "JavaScript";
            case Lua             : return "Lua";
            case CoffeeScript    : return "CoffeeScript";
            case TypeScript      : return "TypeScript";
            case Swift           : return "Swift";
            case Kotlin          : return "Kotlin";
            case ObjectiveC      : return "ObjectiveC";
            case ObjectiveCPlusPlus: return "ObjectiveC++";
            case ActionScript    : return "ActionScript";
            case Scala           : return "Scala";
            default              : return "Unknown";
        }
        return "";
    }


    std::ostream& operator<<(std::ostream& os, SyntaxTokenType& tok)
    {
        os << to_string(tok);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, SyntaxParserLanguage& tok)
    {
        os << to_string(tok);
        return os;
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

    bool SyntaxParser::accept_token()
    {
        // This is meant to be overridden if needed
        return true;
    }

    void SyntaxParser::on_token(std::string& token, bool* discard, bool isSeparator, Token* tok)
    {
        // This is meant to be overridden if needed
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
        return {"+", "-", "*", "/", "=", "<", ">", "!", "?", ":", "::", "->", "<=", ">=", "+=", "-=", "/=", "*=", "^", "^=", "&", "&&", "==", "&=", "||", "|", "%", "%="};
    }

    std::vector<std::string> SyntaxParser::get_keywords()
    {
        SyntaxParserLanguage language = _language;
        // NOTE : The List of Keywords are generated by GitHub Copilot and I have not look through them individually so there might be some problems!
        if(language == SyntaxParserLanguage::C)
        {
            return {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
        }
        else if(language == SyntaxParserLanguage::CPlusPlus)
        {
            return {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "asm", "bool", "catch", "class", "const_cast", "delete", "dynamic_cast", "explicit", "false", "friend", "inline", "mutable", "namespace", "new", "operator", "private", "protected", "public", "reinterpret_cast", "static_cast", "template", "this", "throw", "true", "try", "typeid", "typename", "using", "virtual", "wchar_t"};
        }
        else if(language == SyntaxParserLanguage::Java){
            return {"abstract", "assert", "boolean", "break", "byte", "case", "catch", "char", "class", "const", "continue", "default", "do", "double", "else", "enum", "extends", "final", "finally", "float", "for", "goto", "if", "implements", "import", "instanceof", "int", "interface", "long", "native", "new", "package", "private", "protected", "public", "return", "short", "static", "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try", "void", "volatile", "while"};
        }
        else if(language == SyntaxParserLanguage::Python){
            return {"and", "as", "assert", "break", "class", "continue", "def", "del", "elif", "else", "except", "exec", "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "not", "or", "pass", "print", "raise", "return", "try", "while", "with", "yield"};
        }
        else if(language == SyntaxParserLanguage::CSharp){
            return {"abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue", "decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", "protected", "public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct", "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void", "volatile", "while"};
        }
        else if(language == SyntaxParserLanguage::Rust){
            return {"as", "break", "const", "continue", "crate", "else", "enum", "extern", "false", "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod", "move", "mut", "pub", "ref", "return", "self", "static", "struct", "super", "true", "trait", "type", "unsafe", "use", "where", "while"};
        }
        else if(language == SyntaxParserLanguage::Go){
            return {"break", "case", "chan", "const", "continue", "default", "defer", "else", "fallthrough", "for", "func", "go", "goto", "if", "import", "interface", "map", "package", "range", "return", "select", "switch", "type", "var"};
        }
        else if(language == SyntaxParserLanguage::JavaScript){
            return {"break", "case", "catch", "class", "const", "continue", "debugger", "default", "delete", "do", "else", "export", "extends", "finally", "for", "function", "if", "import", "in", "instanceof", "new", "return", "super", "switch", "this", "throw", "try", "typeof", "var", "void", "while", "with"};
        }
        else if(language == SyntaxParserLanguage::TypeScript){
            return {"break", "case", "catch", "class", "const", "continue", "debugger", "default", "delete", "do", "else", "export", "extends", "finally", "for", "function", "if", "import", "in", "instanceof", "new", "return", "super", "switch", "this", "throw", "try", "typeof", "var", "void", "while", "with"};
        }
        else if(language == SyntaxParserLanguage::CoffeeScript){
            return {"and", "break", "by", "catch", "class", "continue", "debugger", "delete", "do", "each", "else", "extends", "false", "finally", "for", "if", "in", "is", "isnt", "loop", "no", "of", "off", "on", "or", "own", "return", "super", "switch", "then", "this", "throw", "true", "try", "unless", "until", "when", "while", "yes"};
        }
        else if(language == SyntaxParserLanguage::Lua){
            return {"and", "break", "do", "else", "elseif", "end", "false", "for", "function", "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while"};
        }
        else if(language == SyntaxParserLanguage::ActionScript){
            return {"break", "case", "catch", "class", "const", "continue", "default", "delete", "do", "else", "extends", "false", "finally", "for", "function", "if", "implements", "import", "in", "instanceof", "interface", "is", "native", "new", "null", "package", "private", "protected", "public", "return", "static", "super", "switch", "this", "throw", "throws", "transient", "true", "try", "typeof", "use", "var", "void", "while"};
        }
        else if(language == SyntaxParserLanguage::Kotlin){
            return {"as", "break", "class", "continue", "do", "else", "false", "for", "fun", "if", "in", "interface", "is", "null", "object", "package", "return", "super", "this", "throw", "true", "try", "typeof", "val", "var", "when", "while"};
        }
        else if(language == SyntaxParserLanguage::Scala){
            return {"abstract", "case", "catch", "class", "def", "do", "else", "extends", "false", "final", "finally", "for", "forSome", "if", "implicit", "import", "lazy", "match", "new", "null", "object", "override", "private", "protected", "return", "sealed", "super", "this", "throw", "trait", "try", "true", "type", "val", "var", "while"};
        }
        else if(language == SyntaxParserLanguage::Swift){
            return {"as", "break", "case", "catch", "class", "continue", "default", "defer", "do", "else", "fallthrough", "false", "for", "func", "guard", "if", "in", "init", "is", "nil", "new", "return", "self", "Self", "Self.", "super", "switch", "throw", "throws", "true", "try", "typealias", "var", "where", "while"};
        }
        else if(language == SyntaxParserLanguage::ObjectiveC){
            return {"@autoreleasepool", "@catch", "@class", "@dynamic", "@encode", "@end", "@finally", "@implementation", "@import", "@interface", "@optional", "@private", "@property", "@protocol", "@public", "@required", "@selector", "@synchronized", "@synthesize", "@throw", "@try"};
        }
        else if(language == SyntaxParserLanguage::ObjectiveCPlusPlus){
            return {"@autoreleasepool", "@catch", "@class", "@dynamic", "@encode", "@end", "@finally", "@implementation", "@import", "@interface", "@optional", "@private", "@property", "@protocol", "@public", "@required", "@selector", "@synchronized", "@synthesize", "@throw", "@try"};
        }
        
        return {};
    }

    void SyntaxParser::push_token()
    {
        if(_currentToken.type != SyntaxTokenType::None && _currentToken.value.size() > 0)
        {
            if(accept_token())
                _synaxTokens.push_back(_currentToken);
        }
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
        on_token(token, discard, isSeparator, tok);
        return 0;
    }

    bool SyntaxParser::is_number(std::string& s)
    {
        return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return (!std::isdigit(c) && c != '.' && c!='f'); }) == s.end();
    }

#endif
    
}

#endif