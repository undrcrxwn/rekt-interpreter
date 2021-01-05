#pragma once
#include "Pack.h"
#include <memory>

namespace stx
{
    struct Bindings;

    namespace defaults
    {
        namespace impl
        {
            std::shared_ptr<Element> Power(const Element& l, const Element& r);
            std::shared_ptr<Element> Multiply(const Element& l, const Element& r);
            std::shared_ptr<Element> Divide(const Element& l, const Element& r);
            std::shared_ptr<Element> Modulo(const Element& l, const Element& r);
            std::shared_ptr<Element> Add(const Element& l, const Element& r);
            std::shared_ptr<Element> Subtract(const Element& l, const Element& r);
            std::shared_ptr<Element> For(const Element& l, const Element& r);
            std::shared_ptr<Element> Equal(const Element& l, const Element& r);
            std::shared_ptr<Element> NotEqual(const Element& l, const Element& r);
            std::shared_ptr<Element> Greater(const Element& l, const Element& r);
            std::shared_ptr<Element> Less(const Element& l, const Element& r);
            std::shared_ptr<Element> GreaterOrEqual(const Element& l, const Element& r);
            std::shared_ptr<Element> LessOrEqual(const Element& l, const Element& r);
            std::shared_ptr<Element> Assign(const Element& l, const Element& r);
        }
    }

    class Parser
    {
    public:
        Bindings bindings;

        Parser();
        Parser(Bindings& b) : bindings(b) {};
        std::shared_ptr<Pack> Parse(const std::string& s) const;

    protected:
        static Pack FormRawPack(const std::string& s);
        static std::pair<Pack, size_t> FormRawPack(const std::string& s, size_t i);
    };
}