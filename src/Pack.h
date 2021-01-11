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
        void Accept(Visitor& v) override { v.VisitPack(*this); }

        std::ostream& Print(std::ostream& os = std::cout) const;
        std::string ToString() const;

    private:
        std::pair<std::ostream*, size_t> Print(std::ostream& os, size_t tab) const;
    };
}