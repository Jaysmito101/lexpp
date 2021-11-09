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

#ifndef LEXPP_XML_PARSER_H
#define LEXPP_XML_PARSER_H

#include "../lexpp.h"
#include <stack>
#include <regex>

namespace lexpp
{

    struct XMLDocumentNode{
        std::string name = "";
        std::string value = "";
        std::vector<XMLDocumentNode*> children;
        std::vector<std::pair<std::string, std::string>> attributes;
    };

    std::vector<std::pair<std::string, std::string>> stack_to_vector(std::vector<std::pair<std::string, std::string>> stack);

    std::string to_string(XMLDocumentNode* node);

    std::ostream& operator<<(std::ostream& os, XMLDocumentNode* node);

    class XMLParser : public TokenParser
    {
    public:
        XMLParser(std::string data);     

        virtual int process_token(std::string& token, bool* discard, bool isSeparator, Token* tok);

        virtual void on_token(std::string& token, bool* discard, bool isSeparator, Token* tok);

        virtual bool on_node(XMLDocumentNode* node);

        XMLDocumentNode* get_root_node();

    protected:
    // flags
    std::string _lastToken = "";    
    std::stack<std::pair<std::string, std::string>> _attrStack;
    std::stack<XMLDocumentNode*> _nodeStack;
    bool _isInXML = false;
    bool _isInTag = false;
    bool _isInClTag = false;
    bool _isInTagName = false;                   
    bool _isInTagHead = false;                   
    bool _isInTagAttr = false;                   
    };

// Implementations

#ifdef LEXPP_IMPLEMENTATION

    std::vector<std::pair<std::string, std::string>> stack_to_vector(std::stack<std::pair<std::string, std::string>> stack)
    {
        std::vector<std::pair<std::string, std::string>> vec;
        while(!stack.empty())
        {
            vec.push_back(stack.top());
            stack.pop();
        }
        return vec;
    }

    std::string to_string(XMLDocumentNode* node)
    {
        std::string str = "";
        str += "<" + node->name;
        for(auto& attr : node->attributes)
        {
            if(attr.first.size() > 0 && attr.second.size() > 0)
            {
                str += " " + attr.first + "=\"" + attr.second + "\"";
            }
        }
        str += ">" + node->value;
        for(auto& child : node->children)
        {
            str += to_string(child);
        }
        str += "</" + node->name + ">";
        return str;
    }

    std::ostream& operator<<(std::ostream& os, XMLDocumentNode* node)
    {
        os << to_string(node);
        return os;
    }

    XMLParser::XMLParser(std::string data)
    {
        _data = std::regex_replace(data, std::regex("</"), "\\<");
        _data = _data.substr(_data.find_first_of("?>")+2);
        _separators = {" ", "\n", "\t", "\\<", "?", "<", ">", "=", "\""};
        _nodeStack.push(new XMLDocumentNode());
        _nodeStack.top()->name = "xml";
        _includeSeparators = true;
    }

    void XMLParser::on_token(std::string& token, bool* discard, bool isSeparator, Token* tok)
    {
        // This is meant to be overridden if needed
    }

    bool XMLParser::on_node(XMLDocumentNode* node)
    {
        // This is meant to be overridden if needed
        return true;
    }

    XMLDocumentNode* XMLParser::get_root_node()
    {
        _nodeStack.top()->value = "";
        _nodeStack.top()->attributes = std::vector<std::pair<std::string, std::string>>();
        return _nodeStack.top();
    }

    int XMLParser::process_token(std::string& token, bool* discard, bool isSeparator, Token* tok)
    {
        if(isSeparator)
        {
            if(token == "\\<")
            {
                if(_nodeStack.size() == 1)
                    return 0;
                _isInClTag = true;
                _isInTag = false;
                XMLDocumentNode* node = _nodeStack.top();  
                _nodeStack.pop();
                if(on_node(node))
                {
                    _nodeStack.top()->children.push_back(node);
                    _attrStack = std::stack<std::pair<std::string, std::string>>();
                }
                else
                {
                    _nodeStack.push(node);
                }
            }
            else if(token == " ")
            {    
                if(_isInTagName)
                {
                    _isInTagName = false;
                }
                if(_isInTagHead && _isInTagAttr)
                {
                    _attrStack.top().second += " ";
                }
                if(_isInTag)
                {
                    _nodeStack.top()->value += token;
                }
                if(_isInTagHead && !_isInTagAttr)
                {
                    _attrStack.push(std::make_pair("", ""));
                }
            }
            else if(token == "<")
            {
                _attrStack = std::stack<std::pair<std::string, std::string>>();
                XMLDocumentNode* node = new XMLDocumentNode();
                _nodeStack.push(node);
                _isInTagName = true;
                _isInTagHead = true;
                _isInTag = false;
            }
            else if(token == ">")
            {
                if(_isInTagHead)
                {
                    _nodeStack.top()->attributes = stack_to_vector(_attrStack);
                    _isInTag = true;
                    _isInTagName = false;
                    _isInTagHead = false;
                }
            }
            else if(token == "=")
            {
                if(_isInTagHead)
                {
                    _lastToken = "=";
                }
                else
                {
                    _nodeStack.top()->value += token;
                }
            }
            else if(token == "\"")
            {
                if(_isInTagHead)
                {
                    if(_isInTagAttr)
                    {
                        _isInTagAttr = false;
                        _attrStack.push(std::make_pair("", ""));
                    }
                    else
                    {
                        _isInTagAttr = true;
                    }
                }
                else
                {
                    _nodeStack.top()->value += token;
                }
            }
           
        }else{
            if(_isInTag && !_isInTagName)
            {
                _nodeStack.top()->value += token;
            }
            if(_isInTagName)
            {
                _nodeStack.top()->name += token;
            }
            if(_isInTagHead && !_isInTagName)
            {
                if(_isInTagAttr)
                {
                    _attrStack.top().second += token;
                }
                else
                {
                    _attrStack.top().first += token;
                }
            }      
        }
        *discard = true;
        on_token(token, discard, isSeparator, tok);
        return 0;
    }

#endif

}

#endif