#pragma once
#include "element.h"
#include "../bindings.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace rekt
{
    class Token : public Element
    {
    public:
        enum class Type {
            Null,
            Operator,
            Variable,
            Macro,
            Number,
            Function,
            String
        };

        std::string content;

        Element::Type GetElementType() const override { return Element::Type::Token; }
        void Accept(Visitor* v) override { v->Visit(std::dynamic_pointer_cast<Token>(shared_from_this())); }
        virtual Token::Type GetTokenType() const = 0;

        virtual void Normalize() {}
        virtual std::shared_ptr<Element> Resolve() { return shared_from_this(); }
        virtual std::shared_ptr<Element> Clone() override = 0;
        virtual std::string ToString() const override;

    protected:
        static const size_t MaxRecursionDepth = 250;

        Token(const std::string& c) : content(c) {}
    };

    class NullToken : public Token
    {
    public:
        NullToken() : Token("") {}
        Token::Type GetTokenType() const override { return Token::Type::Null; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new NullToken(*this)); }
    };

    class OperatorToken : public Token
    {
    public:
        bool isLazyProcessed;

        OperatorToken(const std::string& c, OptSignature* f, bool lazy) : Token(c), processor(f), isLazyProcessed(lazy) {}
        Token::Type GetTokenType() const override { return Token::Type::Operator; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new OperatorToken(*this)); }
        std::shared_ptr<Element> Process(std::shared_ptr<Element> l, std::shared_ptr<Element> r);

    protected:
        OptSignature* processor;
    };

    class VariableToken : public Token
    {
    public:
        VariableToken(const std::string& c, std::shared_ptr<Token> t) : Token(c), original(t) {}
        Token::Type GetTokenType() const override { return Token::Type::Variable; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new VariableToken(*this)); }

    protected:
        std::shared_ptr<Token> original;
    };

    class MacroToken : public Token
    {
    public:
        MacroToken(const std::string& c, std::function<std::shared_ptr<Element>()>* f) : Token(c), resolver(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Macro; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new MacroToken(*this)); }
        std::shared_ptr<Element> Resolve() override;

    protected:
        std::function<std::shared_ptr<Element>()>* resolver;
    };

    class NumberToken : public Token
    {
    public:
        NumberToken(const std::string& c) : Token(c) {}
        Token::Type GetTokenType() const override { return Token::Type::Number; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new NumberToken(*this)); }

        void Normalize() override
        {
            char* p;
            if (std::isinf(strtod(content.c_str(), &p)) || *p)
                throw std::runtime_error("Parsing exception: number value " + content + " is not valid.");
            
            content.erase(0, content.find_first_not_of('0'));
            if (content.find_first_of('.') != std::string::npos)
                content.erase(content.find_last_not_of('0') + 1, std::string::npos);
            if (content == "")
                content = "0";
            else
            {
                if (content.front() == '.')
                    content.insert(content.begin(), '0');
                if (content.back() == '.')
                    content.pop_back();
            }
        }
    };

    class FunctionToken : public Token
    {
    public:
        FunctionToken(const std::string& c, std::function<void(Pack&)>* f) : Token(c), processor(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Function; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new FunctionToken(*this)); }

    protected:
        std::function<void(Pack&)>* processor;
    };

    class StringToken : public Token
    {
    public:
        StringToken(const std::string& c) : Token(c) {}
        Token::Type GetTokenType() const override { return Token::Type::String; };
        std::shared_ptr<Element> Clone() override { return std::shared_ptr<Element>(new StringToken(*this)); }
    };
};