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
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[1]);

                /* validating signature */
                if (p[1]->GetElementType() != Element::Type::Token ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                std::shared_ptr<Pack> res(new Pack);
                int times = std::abs(((NumberToken&)*p[1]).value);
                for (size_t i = 0; i < times; i++)
                    res->push_back(p[0]->Clone());
                return res;
            }

            std::shared_ptr<Element> Power(Pack& p)
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

                double a = ((NumberToken&)*p[0]).value;
                double b = ((NumberToken&)*p[1]).value;
                return std::make_shared<NumberToken>(std::pow(a, b));
            }

            std::shared_ptr<Element> Multiply(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[0]);
                Core::Process(p[1]);

                if (p[0]->GetElementType() == Element::Type::Token)
                {
                    /* validating signature */
                    if (p[1]->GetElementType() != Element::Type::Token)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    p[0] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[0], false);
                    p[1] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[1], false);

                    if (((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                        ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    double a = ((NumberToken&)*p[0]).value;
                    double b = ((NumberToken&)*p[1]).value;
                    return std::make_shared<NumberToken>(a * b);
                }
                else if (p[0]->GetElementType() == Element::Type::Pack)
                {
                    /* validating signature */
                    if (p[1]->GetElementType() != Element::Type::Token ||
                        ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                        throw std::runtime_error("Processing error: unsupported signature.");

                    Pack args = { p[0], p[1] };
                    return For(args);
                }
                else
                    throw std::runtime_error("Processing error: unsupported signature.");
            }

            std::shared_ptr<Element> Divide(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() != 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                Core::Process(p[0]);
                Core::Process(p[1]);

                /* validating signature */
                if (p[0]->GetElementType() != Element::Type::Token ||
                    p[1]->GetElementType() != Element::Type::Token)
                    throw std::runtime_error("Processing error: unsupported signature.");

                p[0] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[0], false);
                p[1] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[1], false);

                if (((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = ((NumberToken&)*p[0]).value;
                double b = ((NumberToken&)*p[1]).value;
                return std::make_shared<NumberToken>(a / b);
            }

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
                    p[1]->GetElementType() != Element::Type::Token)
                    throw std::runtime_error("Processing error: unsupported signature.");

                p[0] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[0], false);
                p[1] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[1], false);

                if (((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = ((NumberToken&)*p[0]).value;
                double b = ((NumberToken&)*p[1]).value;
                return std::make_shared<NumberToken>(std::fmod(a, b));
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
                    p[1]->GetElementType() != Element::Type::Token)
                    throw std::runtime_error("Processing error: unsupported signature.");

                try
                {
                    p[0] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[0], false);
                    p[1] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[1], false);
                }
                catch (std::exception&)
                {
                    try
                    {
                        p[0] = Convert<Token::Type::String>((std::shared_ptr<Token>&)p[0], true);
                        p[1] = Convert<Token::Type::String>((std::shared_ptr<Token>&)p[1], true);
                    }
                    catch (std::exception&)
                    {
                        throw std::runtime_error("Processing error: unsupported signature.");
                    }

                    std::string& a = ((StringToken&)*p[0]).content;
                    std::string& b = ((StringToken&)*p[1]).content;
                    return std::make_shared<StringToken>(a + b);
                }

                double a = ((NumberToken&)*p[0]).value;
                double b = ((NumberToken&)*p[1]).value;
                return std::make_shared<NumberToken>(a + b);
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
                    p[1]->GetElementType() != Element::Type::Token)
                    throw std::runtime_error("Processing error: unsupported signature.");

                p[0] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[0], false);
                p[1] = Convert<Token::Type::Number>((std::shared_ptr<Token>&)p[1], false);

                if (((Token&)*p[0]).GetTokenType() != Token::Type::Number ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw std::runtime_error("Processing error: unsupported signature.");

                double a = ((NumberToken&)*p[0]).value;
                double b = ((NumberToken&)*p[1]).value;
                return std::make_shared<NumberToken>(a - b);
            }

            std::shared_ptr<Element> Equal(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() < 2)
                    throw std::runtime_error("Processing error: unsupported signature.");

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                bool res = true;
                for (size_t i = 1; i < p.size(); i++)
                {
                    if (*p[0] != *p[i])
                    {
                        res = false;
                        break;
                    }
                }

                return std::make_shared<BooleanToken>(res);
            }

            std::shared_ptr<Element> NotEqual(Pack& p)
            {
                return std::make_shared<BooleanToken>(!((BooleanToken&)Equal(p)).value);
            }

            std::shared_ptr<Element> Greater(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Less(Pack& p) { return nullptr; }

            std::shared_ptr<Element> GreaterOrEqual(Pack& p) { return nullptr; }

            std::shared_ptr<Element> LessOrEqual(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Assign(Pack& p) { return nullptr; }

            std::shared_ptr<Element> Exit(Pack& p) { exit(EXIT_SUCCESS); }
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
            { "operator_=",   impl::Assign         },

            { "exit",         impl::Exit           }
        };

        const OptMap operators = {
            { "for", { nullptr, 0 } },
            { "^",   { nullptr, 1 } },
            { "*",   { nullptr, 2 } },
            { "/",   { nullptr, 2 } },
            { "%",   { nullptr, 2 } },
            { "+",   { nullptr, 3 } },
            { "-",   { nullptr, 3 } },
            { "==",  { nullptr, 4 } },
            { "!=",  { nullptr, 4 } },
            { ">",   { nullptr, 4 } },
            { "<",   { nullptr, 4 } },
            { ">=",  { nullptr, 4 } },
            { "<=",  { nullptr, 4 } },
            { "=",   { nullptr, 5 } }
        };

        const MacroMap macros = {
            { "PI",   []() { return std::make_shared<NumberToken>(3.141592f); } },
            { "RAND", []() { return std::make_shared<NumberToken>(rand() / float(RAND_MAX)); } },
            { "NL",   []() { return std::make_shared<StringToken>("\n"); } },
            { "TIME", []() {
                auto now = std::chrono::system_clock::now().time_since_epoch();
                auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
                return std::make_shared<NumberToken>(double(ns)); } } };
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
