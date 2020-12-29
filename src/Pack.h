#pragma once
#include "Element.h"
#include "Operator.h"
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
        static Pack Parse(const std::string& s);
        std::string ToString() const;
        void Expound() const;
        void Normalize();
        void Resolve();
        // Pack by operators in order of priority.
        void Chop();
        // Invoke operator's processor.
        void Process();

    private:
        std::pair<std::ostream*, size_t> Print(std::ostream& os, size_t tab) const;
        static Pack FormRawPack(const std::string& s);
        static std::pair<Pack, size_t> FormRawPack(const std::string& s, size_t i);
        void PackByOperators(std::vector<Operator::Type> opts);
    };
}