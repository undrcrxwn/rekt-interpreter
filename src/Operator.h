#pragma once
#include "Element.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace stx
{
    class Operator
    {
    public:
        enum class Type
        {
            Power,
            Multiply,
            Divide,
            Modulo,
            Add,
            Subtract,
            For,
            Equal,
            NotEqual,
            Greater,
            Less,
            GreaterOrEqual,
            LessOrEqual,
            Assign
        };

        const Type type;
        const std::string value;
        const std::function<std::shared_ptr<Element>(const Element& l, const Element& r)> execute;
    };

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

    const std::vector<Operator> operators = {
        { Operator::Type::Power,          "^",  impl::Power },
        { Operator::Type::Multiply,       "*",  impl::Multiply },
        { Operator::Type::Divide,         "/",  impl::Divide },
        { Operator::Type::Modulo,         "%",  impl::Modulo },
        { Operator::Type::Add,            "+",  impl::Add },
        { Operator::Type::Subtract,       "-",  impl::Subtract },
        { Operator::Type::For,            "-",  impl::For },
        { Operator::Type::Equal,          "==", impl::Equal },
        { Operator::Type::NotEqual,       "!=", impl::NotEqual },
        { Operator::Type::Greater,        ">",  impl::Greater },
        { Operator::Type::Less,           "<",  impl::Less },
        { Operator::Type::GreaterOrEqual, ">=", impl::GreaterOrEqual },
        { Operator::Type::LessOrEqual,    "<=", impl::LessOrEqual },
        { Operator::Type::Assign,         "=",  impl::Assign }
    };
}