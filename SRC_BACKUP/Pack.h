#pragma once
#include "Element.h"
#include "bindings.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace stx
{
    class Pack : public Element, public std::vector<std::shared_ptr<Element>>
    {
    public:
        Element::Type GetType() const override { return Element::Type::Pack; }

        std::ostream& Print(std::ostream& os = std::cout) const;
        std::string ToString() const;

        void Expound(const Bindings& b) override
        {
            for (std::shared_ptr<Element> e : *this)
                e->Expound(b);
        }

        void Normalize() override
        {
            for (std::shared_ptr<Element> e : *this)
                e->Normalize();
        }

        // Pack by operators in order of priority.
        void Chop(const Bindings& b); //* transfer
        void Resolve(); //* transfer
        // Invoke operator's processor.
        void Process(); //* ?

    private:
        std::pair<std::ostream*, size_t> Print(std::ostream& os, size_t tab) const;
        void PackByOperators(const Bindings& b, std::vector<Operator::Type> ots);
    };
}