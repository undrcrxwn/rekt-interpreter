#pragma once
#include "issue.h"
#include <memory>
#include <string>
#include <vector>

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

        struct Visitor
        {
            virtual void Visit(Token& t) = 0;
            virtual void Visit(Pack& p) = 0;
        };

        std::vector<std::shared_ptr<Issue>> issues;

        virtual bool operator==(const Element& e) const;
        virtual Type GetElementType() const = 0;
        virtual void Accept(Visitor* v) = 0;
        virtual std::shared_ptr<Element> Clone() = 0;
        virtual std::string ToString() const = 0;
        std::shared_ptr<Element> GetDeepest();
    };
}
