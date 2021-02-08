#pragma once
#include "element.h"
#include "../bindings.h"
#include "pack.h"
#include "../utils.h"
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
            Boolean,
            Number,
            Function,
            String
        };

        virtual bool operator==(const Token& t) const { return GetTokenType() == t.GetTokenType(); }
        Element::Type GetElementType() const override { return Element::Type::Token; }
        void Accept(Visitor* v) override { v->Visit(*this); }
        virtual Token::Type GetTokenType() const = 0;

        virtual std::shared_ptr<Element> Process() { return shared_from_this(); }
        virtual std::shared_ptr<Element> Resolve() { return shared_from_this(); }
        virtual std::shared_ptr<Element> Clone() override = 0;
        virtual std::string ToString() const override = 0;

    protected:
        static const size_t MaxRecursionDepth = 250;
    };

    class StringValueBasedToken : public Token
    {
    public:
        std::string content;

        StringValueBasedToken(const std::string& c) : content(c) {}
        bool operator==(const Token& t) const override;
        std::string ToString() const override { return content; }
    };

    class NullToken : public Token
    {
    public:
        Token::Type GetTokenType() const override { return Token::Type::Null; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<NullToken>(*this); }
        std::string ToString() const override { return ""; }
    };

    class OperatorToken : public StringValueBasedToken
    {
    public:
        FuncProcessor* processor;

        OperatorToken(const std::string& c, FuncProcessor* f) : StringValueBasedToken(c), processor(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Operator; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<OperatorToken>(*this); }
    };

    class VariableToken : public StringValueBasedToken
    {
    public:
        std::shared_ptr<Token> original;

        VariableToken(const std::string& c, std::shared_ptr<Token> t) : StringValueBasedToken(c), original(t) {}
        Token::Type GetTokenType() const override { return Token::Type::Variable; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<VariableToken>(*this); }
    };

    class MacroToken : public StringValueBasedToken
    {
    public:
        std::function<std::shared_ptr<Element>()>* resolver;

        MacroToken(const std::string& c, std::function<std::shared_ptr<Element>()>* f) : StringValueBasedToken(c), resolver(f) {}
        Token::Type GetTokenType() const override { return Token::Type::Macro; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<MacroToken>(*this); }
        std::shared_ptr<Element> Resolve() override;
    };

    class BooleanToken : public Token
    {
    public:
        bool value;

        BooleanToken(bool v) : value(v) {}
        bool operator==(const Token& t) const override;
        Token::Type GetTokenType() const override { return Token::Type::Boolean; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<BooleanToken>(*this); }
        std::string ToString() const override { return value ? "true" : "false"; }
    };

    class NumberToken : public Token
    {
    public:
        double value;

        NumberToken(double v) : value(v) {}
        bool operator==(const Token& t) const override;
        Token::Type GetTokenType() const override { return Token::Type::Number; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<NumberToken>(*this); }
        std::string ToString() const override { return std::to_string(value); }
    };

    class FunctionToken : public StringValueBasedToken
    {
    public:
        FuncProcessor* processor;
        Pack arguments;

        FunctionToken(const std::string& c, FuncProcessor* f) : StringValueBasedToken(c), processor(f) {}
        FunctionToken(const std::string& c, FuncProcessor* f, Pack& args) : StringValueBasedToken(c), processor(f), arguments(args) {}
        bool operator==(const Token& t) const override;
        Token::Type GetTokenType() const override { return Token::Type::Function; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<FunctionToken>(content, processor, (Pack&)*arguments.Clone()); }
        std::shared_ptr<Element> Process() override;
        std::shared_ptr<Element> Resolve() override;
    };

    class StringToken : public StringValueBasedToken
    {
    public:
        StringToken(const std::string& c) : StringValueBasedToken(c) {}
        Token::Type GetTokenType() const override { return Token::Type::String; }
        std::shared_ptr<Element> Clone() override { return std::make_shared<StringToken>(*this); }
    };

    template<Token::Type>
    static std::shared_ptr<Token> Convert(std::shared_ptr<Token> t, bool explicitly);

    template<>
    static std::shared_ptr<Token> Convert<Token::Type::Null>(std::shared_ptr<Token> t, bool explicitly)
    {
        switch (t->GetTokenType())
        {
        case Token::Type::Null:
            return t;
        case Token::Type::Number:
            if (((NumberToken&)*t).value == 0)
                return std::make_shared<NullToken>();
            break;
        case Token::Type::String:
            if (((StringToken&)*t).content == "")
                return std::make_shared<NullToken>();
            break;
        }

        if (explicitly)
            return std::make_shared<NullToken>();

        throw std::runtime_error("Invalid conversion.");
    }

    template<>
    static std::shared_ptr<Token> Convert<Token::Type::Operator>(std::shared_ptr<Token> t, bool explicitly)
    {
        switch (t->GetTokenType())
        {
        case Token::Type::Operator:
            return t;
        case Token::Type::Function:
            return std::make_shared<OperatorToken>(
                utils::RemoveFuncNameDecorations(((FunctionToken&)*t).content),
                ((FunctionToken&)*t).processor);
        }
        throw std::runtime_error("Invalid conversion.");
    }

    template<>
    static std::shared_ptr<Token> Convert<Token::Type::Boolean>(std::shared_ptr<Token> t, bool explicitly)
    {
        switch (t->GetTokenType())
        {
        case Token::Type::Boolean:
            return t;
        case Token::Type::Null:
            return std::make_shared<BooleanToken>(false);
        case Token::Type::Number:
            return std::make_shared<BooleanToken>(((NumberToken&)*t).value > 0);
        case Token::Type::String:
            if (explicitly)
            {
                if (((StringToken&)*t).content == "true")
                    return std::make_shared<BooleanToken>(true);
                else if (((StringToken&)*t).content == "false")
                    return std::make_shared<BooleanToken>(false);
                break;
            }
            else
                return std::make_shared<BooleanToken>(((StringToken&)*t).content != "");
        }
        throw std::runtime_error("Invalid conversion.");
    }

    template<>
    static std::shared_ptr<Token> Convert<Token::Type::Number>(std::shared_ptr<Token> t, bool explicitly)
    {
        switch (t->GetTokenType())
        {
        case Token::Type::Number:
            return t;
        case Token::Type::Null:
            return std::make_shared<NumberToken>(0.0f);
        case Token::Type::Boolean:
            return std::make_shared<NumberToken>(((BooleanToken&)*t).value ? 1.0f : 0.0f);
        }

        if (explicitly)
        {
            switch (t->GetTokenType())
            {
            case Token::Type::String:
                return std::make_shared<NumberToken>(std::stod(((StringToken&)*t).content));
            }
        }

        throw std::runtime_error("Invalid conversion.");
    }

    template<>
    static std::shared_ptr<Token> Convert<Token::Type::Function>(std::shared_ptr<Token> t, bool explicitly)
    {
        switch (t->GetTokenType())
        {
        case Token::Type::Function:
            return t;
        case Token::Type::Operator:
            return std::make_shared<FunctionToken>(
                utils::AddFuncNameDecorations(((OperatorToken&)*t).content),
                ((OperatorToken&)*t).processor);
        }
        throw std::runtime_error("Invalid conversion.");
    }

    template<>
    static std::shared_ptr<Token> Convert<Token::Type::String>(std::shared_ptr<Token> t, bool explicitly)
    {
        switch (t->GetTokenType())
        {
        case Token::Type::String:
            return t;
        case Token::Type::Null:
            return std::make_shared<StringToken>("");
        }

        if (explicitly)
        {
            switch (t->GetTokenType())
            {
            case Token::Type::Number:
                return std::make_shared<StringToken>(utils::ToString(((NumberToken&)*t).value));
            }
        }

        throw std::runtime_error("Invalid conversion.");
    }
}
