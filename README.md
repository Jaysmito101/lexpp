<br/>
<p align="center">
    <img src="https://raw.githubusercontent.com/Jaysmito101/lexpp/master/images/ReadmeHeader.png" border="0"></
</p>

<br/>
<p align="center">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Jaysmito101/lexpp?style=for-the-badge">
  <img alt="Lines of code" src="https://img.shields.io/tokei/lines/github/Jaysmito101/lexpp?style=for-the-badge">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/w/Jaysmito101/lexpp?style=for-the-badge">
    <br>
    <img alt="Maintenance" src="https://img.shields.io/maintenance/yes/2021?style=for-the-badge">
    <a href="https://patreon.com/jaysmito101"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Djaysmito101%26type%3Dpledges&style=for-the-badge" alt="Support me on Patreon" /></a>
</p>
<br/>



# lexpp
Small Extremely Powerful Header Only C++ Lexical Analyzer/String Parser Library


Lexpp is made with simplicity and size in mind. The entire library is about 500 lines!

Lexpp is very powerful and can be used for almost all parsing needs!

You can check the <a  href="https://github.com/Jaysmito101/lexpp/tree/master/examples">examples/</a> for more elaborate usage.

# How to Use

Just place the `lexpp.h` file in your project include directory.

In one cpp file define `LEXPP_IMPLEMENTATION` before importing lexpp like  this:


    #define LEXPP_IMPLEMENTATION
    #include "lexpp.h"
    
You are all done to use lexpp!



# Basic Examples

### String Parsing

    std::string data = "some text to parse! ";
    std::vector<std::string> tokens = lexpp::lex(data, " ;\n");

    for(std::string& token : tokens){
        std::cout << token << std::endl;
    }

### Some more string parsing

    std::string data = "some text to parse! ";
    std::vector<std::string> tokens = lexpp::lex(data, {"<=", "<<", "\n", "::", ",", "}", "{", ";", " "}, false);

    for(std::string& token : tokens){
        std::cout << token << std::endl;
    }


### Using Custom Token Classifier


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


Now the Lexing

    std::vector<std::string> keywords = {"for", "void", "return", "if", "int"};
    std::vector<lexpp::Token> tokens = lexpp::lex(data, {"<=", "<<", "\n", "::", ",", "}", "{", "(", ")" ";", " "}, [keywords](std::string& token, bool* discard, bool is_separator) -> int {
        if(std::find(keywords.begin(), keywords.end(), token) != keywords.end()){
            return MyTokens::Keyword;
        }
        if(is_number(token))
            return MyTokens::Number;
        else
            return MyTokens::String;
    }, false);

    for(lexpp::Token& token : tokens){
        std::cout << TokenToString(token.type) << " -> " << token.value << std::endl;
    }
