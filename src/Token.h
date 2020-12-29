#pragma once
#include "Element.h"
#include <string>

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

        // Determine the token type if not defined.
        void Expound();
        // Process escape sequences in a string token.
        void Normalize();
        // Resolve var and macro contents.
        void Resolve();
    };
}