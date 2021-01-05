﻿#pragma once
#include "Element.h"
#include "bindings.h"
#include <string>
#include <cmath>
#include <algorithm>

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
        //? todo: error info
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
            if (b.operators.find(content) != b.operators.end())
            {
                type = Token::Type::Operator;
                return;
            }

            // variable
            if (content[0] == '$' && content.size() > 1)
            {
                type = Token::Type::Variable;
                return;
            }

            // macro
            if (b.macros.find(content) != b.macros.end())
            {
                type = Token::Type::Macro;
                return;
            }

            // special string cases
            std::string lowercase(content);
            std::transform(content.begin(), content.end(), lowercase.begin(),
                [](char c) { return std::tolower(c); });
            if(lowercase == "inf" || lowercase == "nan")
            {
                type = Token::Type::String;
                return;
            }

            // number
            char* p;
            double result = strtod(content.c_str(), &p);
            if (std::isinf(result))
            {
                //? todo: throw error (limit exceeded)
                type = Token::Type::Null;
                return;
            }

            if (!*p)
            {
                type = Token::Type::Number;
                return;
            }

            // function
            if (b.functions.find(content) != b.functions.end())
            {
                type = Token::Type::Function;
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