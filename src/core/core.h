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
        void Execute(std::shared_ptr<Pack>& p);
        void Process(std::shared_ptr<Pack>& p);
        void Resolve(std::shared_ptr<Pack>& p);
    };

    class ProcessingVisitor : public Element::Visitor
    {
    public:
        void Visit(std::shared_ptr<Token>&& t) const override {}
        void Visit(std::shared_ptr<Pack>&& p) const override;
    };
    
    class ResolvingVisitor : public Element::Visitor
    {
    public:
        void Visit(std::shared_ptr<Token>&& t) const override {}
        void Visit(std::shared_ptr<Pack>&& p) const override;
    };
}