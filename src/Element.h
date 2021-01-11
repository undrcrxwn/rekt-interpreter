#pragma once

namespace stx
{
    class Token;
    class Pack;

    class Element
    {
    public:
        enum class Type
        {
            Token,
            Pack
        };

        class Visitor {
        public:
            virtual void VisitToken(Token& t) const = 0;
            virtual void VisitPack(Pack& p) const = 0;
        };

        virtual Type GetElementType() const = 0;
        virtual void Accept(Visitor& v) = 0;
    };
}