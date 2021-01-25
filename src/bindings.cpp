#include "bindings.h"
#include "element/token.h"
#include "element/pack.h"
#include "core/core.h"
#include <chrono>
#include <math.h>

namespace rekt
{
    namespace defaults
    {
        namespace impl
        {
            std::shared_ptr<Element> Power(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }

            std::shared_ptr<Element> Multiply(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
            {
                if (l->GetElementType() == Element::Type::Token)
                {
                    /* early resolving */
                    ResolvingVisitor resolver;
                    l->Accept(&resolver);
                    r->Accept(&resolver);

                    /* validating signature */
                    if (r->GetElementType() != Element::Type::Token ||
                        ((Token&)*l).GetTokenType() != Token::Type::Number ||
                        ((Token&)*r).GetTokenType() != Token::Type::Number)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    double a = atof(((Token&)*l).content.c_str());
                    double b = atof(((Token&)*r).content.c_str());
                    std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(a * b)));
                    return res;
                }
                else if (l->GetElementType() == Element::Type::Pack)
                {
                    /* early resolving */
                    ResolvingVisitor resolver;
                    l->Accept(&resolver);
                    r->Accept(&resolver);

                    /* validating signature */
                    if (r->GetElementType() != Element::Type::Token ||
                        ((Token&)*r).GetTokenType() != Token::Type::Number)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    std::shared_ptr<Element> res = For(l, r);
                    return res;
                }
            }

            std::shared_ptr<Element> Divide(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }

            std::shared_ptr<Element> Modulo(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
            {
                /* early resolving */
                ResolvingVisitor resolver;
                l->Accept(&resolver);
                r->Accept(&resolver);

                /* validating signature */
                if (l->GetElementType() != Element::Type::Token ||
                    r->GetElementType() != Element::Type::Token ||
                    ((Token&)*l).GetTokenType() != Token::Type::Number ||
                    ((Token&)*r).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = atof(((Token&)*l).content.c_str());
                double b = atof(((Token&)*r).content.c_str());
                std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(std::fmod(a, b))));
                return res;
            }

            std::shared_ptr<Element> Add(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
            {
                /* early resolving */
                ResolvingVisitor resolver;
                l->Accept(&resolver);
                r->Accept(&resolver);

                /* validating signature */
                if (l->GetElementType() != Element::Type::Token ||
                    r->GetElementType() != Element::Type::Token ||
                    ((Token&)*l).GetTokenType() != Token::Type::Number ||
                    ((Token&)*r).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = atof(((Token&)*l).content.c_str());
                double b = atof(((Token&)*r).content.c_str());
                std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(a + b)));
                return res;
            }

            std::shared_ptr<Element> Subtract(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
            {
                /* early resolving */
                ResolvingVisitor resolver;
                l->Accept(&resolver);
                r->Accept(&resolver);

                /* validating signature */
                if (l->GetElementType() != Element::Type::Token ||
                    r->GetElementType() != Element::Type::Token ||
                    ((Token&)*l).GetTokenType() != Token::Type::Number ||
                    ((Token&)*r).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = atof(((Token&)*l).content.c_str());
                double b = atof(((Token&)*r).content.c_str());
                std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(a - b)));
                return res;
            }

            std::shared_ptr<Element> For(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
            {
                /* early resolving */
                ResolvingVisitor resolver;
                r->Accept(&resolver);

                /* validating signature */
                if (r->GetElementType() != Element::Type::Token ||
                    ((Token&)*r).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                NumberToken& num = (NumberToken&)*r;
                std::shared_ptr<Pack> res(new Pack);
                int times = std::abs(atoi(num.content.c_str()));
                for (size_t i = 0; i < times; i++)
                    res->push_back(l->Clone());
                return res;
            }

            std::shared_ptr<Element> Equal(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
            std::shared_ptr<Element> NotEqual(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
            std::shared_ptr<Element> Greater(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
            std::shared_ptr<Element> Less(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
            std::shared_ptr<Element> GreaterOrEqual(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
            std::shared_ptr<Element> LessOrEqual(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
            std::shared_ptr<Element> Assign(std::shared_ptr<Element> l, std::shared_ptr<Element> r) { return nullptr; }
        }

        const OptMap operators = {
            { "for", { impl::For,            0,  true  } },
            { "^",   { impl::Power,          1,  false } },
            { "*",   { impl::Multiply,       2,  false } },
            { "/",   { impl::Divide,         2,  false } },
            { "%",   { impl::Modulo,         2,  false } },
            { "+",   { impl::Add,            3,  false } },
            { "-",   { impl::Subtract,       3,  false } },
            { "==",  { impl::Equal,          4,  false } },
            { "!=",  { impl::NotEqual,       4,  false } },
            { ">",   { impl::Greater,        4,  false } },
            { "<",   { impl::Less,           4,  false } },
            { ">=",  { impl::GreaterOrEqual, 4,  false } },
            { "<=",  { impl::LessOrEqual,    4,  false } },
            { "=",   { impl::Assign,         5,  false } }
        };

        const MacroMap macros = {
            { "PI",   []() { return std::make_shared<NumberToken>("3.141592"); } },
            { "RAND", []() { return std::make_shared<NumberToken>(std::to_string(rand() / float(RAND_MAX))); } },
            { "NL",   []() { return std::make_shared<StringToken>("\n"); } },
            { "TIME", []() {
                auto now = std::chrono::system_clock::now().time_since_epoch();
                auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
                return std::make_shared<NumberToken>(std::to_string(nano)); } }
        };
    }

    Bindings::Bindings()
    {
        operators = defaults::operators;
        macros = defaults::macros;
    }
}