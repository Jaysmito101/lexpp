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
    <a href="https://patreon.com/jaysmito101"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Djaysmito101%26type%3Dpledges&style=for-the-badge" alt="Support me on Patreon" /></a>
</p>
<br/>



# lexpp
Small Extremely Powerful Header Only C++ Lexical Analyzer/String Parser Library


Lexpp is made with simplicity and size in mind. The entire library is about 500 lines!

Lexpp is very powerful and can be used for almost all parsing needs!

You can check the <a  href="https://github.com/Jaysmito101/lexpp/tree/master/examples">examples/</a> for more elaborate usage.

# Docs : 
### https://github.com/Jaysmito101/lexpp/wiki
### For any help join our discord server https://discord.gg/muq5nDxF9t

# How to Use

Just place the `lexpp.h` file in your project include directory.

In one cpp file define `LEXPP_IMPLEMENTATION` before importing lexpp like  this:


    #define LEXPP_IMPLEMENTATION
    #include "lexpp.h"
    
You are all done to use lexpp!


# Basic Examples

## String parsing
<details>
    <summary> Click To See Code </summary>
    
    std::string data = "some text to parse! ";
    std::vector<std::string> tokens = lexpp::lex(data, {"<=", "<<", "\n", "::", ",", "}", "{", ";", " "}, false);

    for(std::string& token : tokens){
        std::cout << token << std::endl;
    }
        
 </details>


## Using Custom Token Classifier
        
Some Structs we will need
<details>
        <summary> Click To See Code </summary>
        
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
        
</details>

Now the Lexing
        
<details>
        <summary> Click To See Code </summary>
        
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
    
</details>

## Using the `TokenParser` class

We need to extend the `TokenParser` class to have our cuastom token parser

<details>
        <summary> Click To See Code </summary>
        
    class MyTokenParser : public lexpp::TokenParser
    {
    public:
    MyTokenParser(std::string data, std::string separators)
    :TokenParser(data, separators, false){}

    virtual int process_token(std::string& token, bool* discard, bool isSeparator) override
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
        
</details>
    
Now using the class with the lexer

<details>
        <summary> Click To See Code </summary>
            
    std::vector<lexpp::Token> tokens =     lexpp::lex(std::make_shared<MyTokenParser>(data, "\n :,[]{}().\t"));
    for(lexpp::Token& token : tokens){
        std::cout << TokenToString(token.type) << " -> " << token.value << std::endl;
    }
            
</details>
            
## Making an email parser with lexpp

First a struct to store out data
            
<details>
        <summary> Click To See Code </summary>     
    
    struct Email{
        std::string name;
        std::string domainFront;
        std::string domainEnd;
        std::string domain;
    };
    
</details>
    
Now we need to make our custom token parser for email parsing

<details>
        <summary> Click To See Code </summary>
      
    class EmailTokenParser : public lexpp::TokenParser
    {
    public:
    EmailTokenParser(std::string data, std::string separators = "\n@.")
    :TokenParser(data, separators, true){}

    virtual int process_token(std::string& token, bool* discard, bool isSeparator) override
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
      
</details>
    
Now finally calling lex
<details>
        <summary> Click To See Code </summary>
    
    std::shared_ptr<EmailTokenParser> tok_parser = std::make_shared<EmailTokenParser>(data+"\n", "\n@.");
    lexpp::lex(tok_parser);
    for(Email& email : tok_parser->emailIds){
        std::cout << "Email : \nNAME: " << email.name << "\nDOMAIN : " << email.domain << std::endl;
    }
    
</details>

# Support

I am just a Highschool student so I may not have the best quality of code but still i am trying my best to write good code!

Any support would be highly appretiated!

For example you could add a feature and contribute via pull requests or you could even report any issues with the program!

And the best thing you could do to support this project is spread word about this so that more people who might be interested in this may use this!

Please considering tweeting about this! 


<img alt="Visitors" src="https://visitor-badge.glitch.me/badge?page_id=Jaysmito101.lexpp&left_color=gray&right_color=green&style=for-the-badge">
