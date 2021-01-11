#pragma once
#include "Element.h"
#include "bindings.h"
#include <string>
#include <cmath>
#include <algorithm>

namespace stx
{
    class Token : public Element, std::enable_shared_from_this<Token>
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

        //? todo: error info
        std::string content;

        Element::Type GetElementType() const override { return Element::Type::Token; }
        void Accept(Visitor& v) override { v.VisitToken(*this); }
        virtual Token::Type GetTokenType() const = 0;

        virtual void Normalize() {}
        virtual std::shared_ptr<Element> Resolve() { return shared_from_this(); }

    protected:
        Token(const std::string& c) : content(c) {}
    };

    class NullToken : public Token
    {
    public:
        NullToken() : Token("") {}
        Token::Type GetTokenType() const override { return Token::Type::Null; };
    };

    class OperatorToken : public Token
    {
    public:
        OperatorToken(const std::string& c, OptSignature* f) : Token(c), processor(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Operator; };

    protected:
        OptSignature* processor;
    };

    class VariableToken : public Token
    {
    public:
        VariableToken(const std::string& c, std::shared_ptr<Token> t) : Token(c), original(t) {}
        Token::Type GetTokenType() const override { return Token::Type::Variable; };

    protected:
        std::shared_ptr<Token> original;
    };

    class MacroToken : public Token
    {
    public:
        MacroToken(const std::string& c, std::function<std::shared_ptr<Token>()>* f) : Token(c), resolver(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Macro; };

    protected:
        std::function<std::shared_ptr<Token>()>* resolver;
    };

    class NumberToken : public Token
    {
    public:
        NumberToken(const std::string& c) : Token(c) {}
        Token::Type GetTokenType() const override { return Token::Type::Number; };
    };

    class FunctionToken : public Token
    {
    public:
        FunctionToken(const std::string& c, std::function<void(Pack&)>* f) : Token(c), processor(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Function; };

    protected:
        std::function<void(Pack&)>* processor;
    };

    class StringToken : public Token
    {
    public:
        StringToken(const std::string& c) : Token(c) {}
        Token::Type GetTokenType() const override { return Token::Type::String; };
    };
};