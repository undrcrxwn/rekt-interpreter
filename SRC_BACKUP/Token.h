#pragma once
#include "Element.h"
#include "bindings.h"
#include <string>
#include <cmath>

namespace stx
{
    class Token : public Element
    {
    public:
        enum class Type {
            Unknown,
            Function,
            Number,
            Null,
            Variable,
            Macro,
            String,
            Operator
        };

        Type type;
        //? todo: error;
        std::string content;

        Token(Type t = Type::Unknown, const std::string& c = "") : type(t), content(c) {}
        Element::Type GetType() const override { return Element::Type::Token; }

        void Expound(const Bindings& b) override
        {
            if (type != Token::Type::Unknown)
                return;

            // null
            if (content == "" || content == "NULL")
            {
                type = Token::Type::Null;
                return;
            }

            // operator
            bool found = std::find_if(b.operators.begin(), b.operators.end(), [&](const Operator& o)
                { return o.key == content; }) != b.operators.end();
            if (found)
            {
                type = Token::Type::Operator;
                return;
            }

            // number
            char* p;
            double result = strtod(content.c_str(), &p);
            if (std::isinf(result))
            {
                type = Token::Type::Null;
                //? todo: throw error (limit exceeded)
                return;
            }

            if (!*p)
            {
                type = Token::Type::Number;
                return;
            }

            // function
            found = std::find_if(b.functions.begin(), b.functions.end(), [&](const Function& f)
                { return f.key == content; }) != b.functions.end();
            if (found)
            {
                type = Token::Type::Function;
                return;
            }

            // variable
            if (content[0] == '$' &&
                std::count(content.begin(), content.end(), '$') == 1)
            {
                type = Token::Type::Variable;
                return;
            }

            // macro
            found = std::find_if(b.macros.begin(), b.macros.end(), [&](const Macro& m)
                { return m.key == content; }) != b.macros.end();
            if (found)
            {
                type = Token::Type::Macro;
                return;
            }

            // string
            type = Token::Type::String;
        }

        void Normalize() override
        {
            switch (type)
            {
            case Type::String:
                for (size_t i = 0; i < content.size(); i++)
                {
                    if (content[i] == '\\')
                        content.erase(content.begin() + i);
                }
                break;
            }
        }

        // Resolve var and macro contents.
        void Resolve();
    };
};