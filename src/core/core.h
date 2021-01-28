#pragma once
#include "../bindings.h"
#include "../element/element.h"
#include <vector>

namespace rekt
{
    class Core
    {
    public:
        Bindings bindings;

        Core() = default;
        Core(Bindings& b) : bindings(b) {};
        void Execute(Pack& p);
        void PackOperators(Element& e);
        static void Process(std::shared_ptr<Element>& e);
        static void Process(Pack& p);
        static void Resolve(std::shared_ptr<Element>& e);
        static void Resolve(Pack& p);
    };

    class OperatorPacker : public Element::Visitor
    {
    public:
        OperatorPacker(const std::vector<std::string>& opts) : optGroup(opts) {}
        void Visit(Token& t) override;
        void Visit(Pack& p) override;

    protected:
        const std::vector<std::string>& optGroup;
    };
}