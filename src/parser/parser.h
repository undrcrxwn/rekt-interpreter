#pragma once
#include "../bindings.h"
#include "../element/element.h"
#include "../element/pack.h"
#include "../element/token.h"
#include <memory>
#include <vector>

namespace rekt
{
    class Parser
    {
    public:
        Bindings bindings;

        Parser() = default;
        Parser(Bindings& b) : bindings(b) {};
        std::shared_ptr<Pack> Parse(const std::string& s);

    private:
        std::shared_ptr<Pack> FormRawPack(const std::string& s);
        std::pair<std::shared_ptr<Pack>, size_t> FormRawPack(const std::string& s, size_t i);
        std::shared_ptr<Token> ParseToken(const std::string& s);
    };

    class OperatorPackingVisitor : public Element::Visitor
    {
    public:
        OperatorPackingVisitor(const std::vector<std::string>& ots) : optGroup(ots) {}
        void Visit(std::shared_ptr<Token>&& t) const override {}
        void Visit(std::shared_ptr<Pack>&& p) const override;

    protected:
        const std::vector<std::string> optGroup;
    };
}