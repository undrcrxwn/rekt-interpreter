#pragma once
#include "element.h"
#include "../bindings.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace rekt
{
    class Pack : public Element, public std::vector<std::shared_ptr<Element>>
    {
    public:
        Element::Type GetElementType() const override { return Element::Type::Pack; }
        void Accept(Visitor* v) override { v->Visit(std::dynamic_pointer_cast<Pack>(shared_from_this())); }
        std::shared_ptr<Element> Clone() override
        {
            std::shared_ptr<Pack> res(new Pack);
            for (std::shared_ptr<Element>& e : *this)
                res->push_back(e->Clone());
            return res;
        }

        std::ostream& Print(std::ostream& os = std::cout) const;
        std::string ToString() const override;

    private:
        std::pair<std::ostream*, size_t> Print(std::ostream& os, size_t tab) const;
    };
}