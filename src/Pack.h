#pragma once
#include "Element.h"
#include "Bindings.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace stx
{
    class Pack : public Element, public std::vector<std::shared_ptr<Element>>
    {
    public:
        Element::Type GetElementType() const override { return Element::Type::Pack; }

        std::ostream& Print(std::ostream& os = std::cout) const;
        std::string ToString() const;

        void Normalize() override
        {
            for (std::shared_ptr<Element> e : *this)
                e->Normalize();
        }

        // Pack by operators in order of priority.
        void Chop(const OptMap& ots);
        void Resolve();
        // Invoke operator's processor.
        void Process();

    private:
        std::pair<std::ostream*, size_t> Print(std::ostream& os, size_t tab) const;
        void PackByOperators(const std::vector<std::string>& ots);
    };
}