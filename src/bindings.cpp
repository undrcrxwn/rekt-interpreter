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
            std::shared_ptr<Element> For(Pack& p)
            {
                /* validating arguments amount */
                if(p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[1]);

                /* validating signature */
                if (p[1]->GetElementType() != Element::Type::Token ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                NumberToken& num = (NumberToken&)*p[1];
                std::shared_ptr<Pack> res(new Pack);
                int times = std::abs(atoi(num.content.c_str()));
                for (size_t i = 0; i < times; i++)
                    res->push_back(p[0]->Clone());

                return res;
            }

            std::shared_ptr<Element> Power(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Multiply(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                if (p[0]->GetElementType() == Element::Type::Token)
                {
                    /* early processing */
                    Core::Process(p[0]);
                    Core::Process(p[1]);

                    /* validating signature */
                    if (p[1]->GetElementType() != Element::Type::Token ||
                        ((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                        ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    double a = atof(((Token&)*p[0]).content.c_str());
                    double b = atof(((Token&)*p[1]).content.c_str());
                    std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(a * b)));
                    return res;
                }
                else if (p[0]->GetElementType() == Element::Type::Pack)
                {
                    /* early processing */
                    Core::Process(p[0]);
                    Core::Process(p[1]);

                    /* validating signature */
                    if (p[1]->GetElementType() != Element::Type::Token ||
                        ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    Pack args = { p[0], p[1] };
                    std::shared_ptr<Element> res = For(args);
                    return res;
                }
            }

            std::shared_ptr<Element> Divide(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Modulo(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[0]);
                Core::Process(p[1]);

                /* validating signature */
                if (p[0]->GetElementType() != Element::Type::Token ||
                    p[1]->GetElementType() != Element::Type::Token ||
                    ((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = atof(((Token&)*p[0]).content.c_str());
                double b = atof(((Token&)*p[1]).content.c_str());
                std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(std::fmod(a, b))));
                return res;
            }

            std::shared_ptr<Element> Add(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[0]);
                Core::Process(p[1]);

                /* validating signature */
                if (p[0]->GetElementType() != Element::Type::Token ||
                    p[1]->GetElementType() != Element::Type::Token ||
                    ((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = atof(((Token&)*p[0]).content.c_str());
                double b = atof(((Token&)*p[1]).content.c_str());
                std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(a + b)));

                return res;
            }

            std::shared_ptr<Element> Subtract(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[0]);
                Core::Process(p[1]);
                
                /* validating signature */
                if (p[0]->GetElementType() != Element::Type::Token ||
                    p[1]->GetElementType() != Element::Type::Token ||
                    ((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = atof(((Token&)*p[0]).content.c_str());
                double b = atof(((Token&)*p[1]).content.c_str());
                std::shared_ptr<NumberToken> res(new NumberToken(std::to_string(a - b)));
                return res;
            }

            std::shared_ptr<Element> Equal(Pack& p) { return nullptr; }

            std::shared_ptr<Element> NotEqual(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Greater(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Less(Pack& p) { return nullptr; }

            std::shared_ptr<Element> GreaterOrEqual(Pack& p) { return nullptr; }

            std::shared_ptr<Element> LessOrEqual(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Assign(Pack& p) { return nullptr; }
        }

        const FuncMap functions = {
            { "operator_for", impl::For            },
            { "operator_^",   impl::Power          },
            { "operator_*",   impl::Multiply       },
            { "operator_/",   impl::Divide         },
            { "operator_%",   impl::Modulo         },
            { "operator_+",   impl::Add            },
            { "operator_-",   impl::Subtract       },
            { "operator_==",  impl::Equal          },
            { "operator_!=",  impl::NotEqual       },
            { "operator_>",   impl::Greater        },
            { "operator_<",   impl::Less           },
            { "operator_>=",  impl::GreaterOrEqual },
            { "operator_<=",  impl::LessOrEqual    },
            { "operator_=",   impl::Assign         }
        };

        const OptMap operators = {
            { "for", { nullptr, 0, true  } },
            { "^",   { nullptr, 1, false } },
            { "*",   { nullptr, 2, false } },
            { "/",   { nullptr, 2, false } },
            { "%",   { nullptr, 2, false } },
            { "+",   { nullptr, 3, false } },
            { "-",   { nullptr, 3, false } },
            { "==",  { nullptr, 4, false } },
            { "!=",  { nullptr, 4, false } },
            { ">",   { nullptr, 4, false } },
            { "<",   { nullptr, 4, false } },
            { ">=",  { nullptr, 4, false } },
            { "<=",  { nullptr, 4, false } },
            { "=",   { nullptr, 5, false } }
        };

        const MacroMap macros = {
            { "PI",   []() { return std::make_shared<NumberToken>("3.141592"); } },
            { "RAND", []() { return std::make_shared<NumberToken>(std::to_string(rand() / float(RAND_MAX))); } },
            { "NL",   []() { return std::make_shared<StringToken>("\n"); } },
            { "TIME", []() {
                auto now = std::chrono::system_clock::now().time_since_epoch();
                auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
                return std::make_shared<NumberToken>(std::to_string(nano)); } } };
    }

    Bindings::Bindings()
    {
        functions = defaults::functions;
        operators = defaults::operators;
        for (auto& f : functions)
        {
            if (f.first.rfind("operator_", 0) == 0)
            {
                std::string s = f.first.substr(9);
                operators[s].processor = &functions[f.first];
            }
        }
        macros = defaults::macros;
    }
}