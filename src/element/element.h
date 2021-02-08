#pragma once
#include <memory>
#include <string>

namespace rekt
{
    class Token;
    class Pack;

    class Element : public std::enable_shared_from_this<Element>
    {
    public:
        enum class Type
        {
            Token,
            Pack
        };

        class Visitor {
        public:
            virtual void Visit(Token& t) = 0;
            virtual void Visit(Pack& p) = 0;
        };

        virtual bool operator==(const Element& e) const;
        virtual Type GetElementType() const = 0;
        virtual void Accept(Visitor* v) = 0;
        virtual std::shared_ptr<Element> Clone() = 0;
        virtual std::string ToString() const = 0;
        std::shared_ptr<Element> GetDeepest();
    };
}