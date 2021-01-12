#pragma once
#include "../bindings.h"
#include "../element/element.h"
#include "../element/pack.h"
#include "../element/token.h"
#include <memory>
#include <vector>

namespace stx
{
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

    class OperatorPackingVisitor : public Element::Visitor
    {
    public:
        OperatorPackingVisitor(const std::vector<std::string>& ots) : optGroup(ots) {}
        void VisitToken(Token& t) const override {}
        void VisitPack(Pack& p) const override;

    protected:
        const std::vector<std::string> optGroup;
    };

    class Parser
    {
    public:
        Bindings bindings;

        Parser();
        Parser(Bindings& b) : bindings(b) {};
        std::shared_ptr<Pack> Parse(const std::string& s);

    private:
        std::shared_ptr<Pack> FormRawPack(const std::string& s);
        std::pair<std::shared_ptr<Pack>, size_t> FormRawPack(const std::string& s, size_t i);
        std::shared_ptr<Token> ParseToken(const std::string& s);
    };
}