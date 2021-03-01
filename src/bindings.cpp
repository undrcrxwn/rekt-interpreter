#include "bindings.h"
#include "element/token.h"
#include "element/pack.h"
#include "core/core.h"
#include <chrono>
#include <thread>
#include <math.h>
#include <boost/optional.hpp>

namespace rekt
{
    namespace defaults
    {
        namespace impl
        {
            boost::optional<std::shared_ptr<Element>> For(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() != 2) throw WrongSignatureException();

                /* early processing */
                Core::Process(p[1]);

                /* validating signature */
                if (p[1]->GetElementType() != Element::Type::Token ||
                    ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                    throw WrongSignatureException();

                std::shared_ptr<Pack> res(new Pack);
                int times = std::abs(((NumberToken&)*p[1]).value);
                for (size_t i = 0; i < times; i++)
                    res->push_back(p[0]->Clone());
                return std::static_pointer_cast<Element>(res);
            }

            boost::optional<std::shared_ptr<Element>> Power(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() < 2) throw WrongSignatureException();

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                /* validating signature */
                for (std::shared_ptr<Element>& e : p)
                    if (e->GetElementType() != Element::Type::Token ||
                        ((Token&)*e).GetTokenType() != Token::Type::Number)
                        throw WrongSignatureException();

                double res = ((NumberToken&)*p[0]).value;
                for (size_t i = 1; i < p.size(); i++)
                    res = std::pow(res, ((NumberToken&)*p[i]).value);
                return std::static_pointer_cast<Element>(std::make_shared<NumberToken>(res));
            }

            boost::optional<std::shared_ptr<Element>> Multiply(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() < 2) throw WrongSignatureException();

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                if (p[0]->GetElementType() == Element::Type::Token)
                {
                    /* validating signature */
                    if (p[1]->GetElementType() != Element::Type::Token)
                        throw WrongSignatureException();

                    for (std::shared_ptr<Element>& e : p)
                        e = Convert<Token::Type::Number>(std::static_pointer_cast<Token>(e), false);

                    for (std::shared_ptr<Element>& e : p)
                        if (((Token&)*e).GetTokenType() != Token::Type::Number)
                            throw WrongSignatureException();

                    double res = 1;
                    for (std::shared_ptr<Element>& e : p)
                        res *= ((NumberToken&)*e).value;
                    return std::static_pointer_cast<Element>(std::make_shared<NumberToken>(res));
                }
                else if (p[0]->GetElementType() == Element::Type::Pack)
                {
                    /* validating signature */
                    if (p[1]->GetElementType() != Element::Type::Token ||
                        ((Token&)*p[1]).GetTokenType() != Token::Type::Number)
                        throw WrongSignatureException();

                    Pack args = { p[0], p[1] };
                    return For(args);
                }
                else
                    throw WrongSignatureException();
            }

            boost::optional<std::shared_ptr<Element>> Divide(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() < 2) throw WrongSignatureException();

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                /* validating signature */
                for (std::shared_ptr<Element>& e : p)
                    if (e->GetElementType() != Element::Type::Token)
                        throw WrongSignatureException();

                for (std::shared_ptr<Element>& e : p)
                    e = Convert<Token::Type::Number>(std::static_pointer_cast<Token>(e), false);

                for (std::shared_ptr<Element>& e : p)
                    if (((Token&)*e).GetTokenType() != Token::Type::Number)
                        throw WrongSignatureException();

                double res = ((NumberToken&)*p[0]).value;
                for (size_t i = 1; i < p.size(); i++)
                    res /= ((NumberToken&)*p[i]).value;
                return std::static_pointer_cast<Element>(std::make_shared<NumberToken>(res));
            }

            boost::optional<std::shared_ptr<Element>> Modulo(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() < 2) throw WrongSignatureException();

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                /* validating signature */
                for (std::shared_ptr<Element>& e : p)
                    if (e->GetElementType() != Element::Type::Token)
                        throw WrongSignatureException();

                for (std::shared_ptr<Element>& e : p)
                    e = Convert<Token::Type::Number>(std::static_pointer_cast<Token>(e), false);

                for (std::shared_ptr<Element>& e : p)
                    if (((Token&)*e).GetTokenType() != Token::Type::Number)
                        throw WrongSignatureException();

                double res = ((NumberToken&)*p[0]).value;
                for (size_t i = 1; i < p.size(); i++)
                    res = std::fmod(res, ((NumberToken&)*p[i]).value);
                return std::static_pointer_cast<Element>(std::make_shared<NumberToken>(res));
            }

            boost::optional<std::shared_ptr<Element>> Add(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() < 2) throw WrongSignatureException();

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                /* validating signature */
                for (std::shared_ptr<Element>& e : p)
                    if (e->GetElementType() != Element::Type::Token)
                        throw WrongSignatureException();

                try
                {
                    for (std::shared_ptr<Element>& e : p)
                        e = Convert<Token::Type::Number>(std::static_pointer_cast<Token>(e), false);
                }
                catch (std::exception&)
                {
                    try
                    {
                        for (std::shared_ptr<Element>& e : p)
                            e = Convert<Token::Type::String>(std::static_pointer_cast<Token>(e), true);
                    }
                    catch (std::exception&)
                    {
                        throw WrongSignatureException();
                    }

                    std::string res;
                    for (std::shared_ptr<Element>& e : p)
                        res += ((StringToken&)*e).content;
                    return std::static_pointer_cast<Element>(std::make_shared<StringToken>(res));
                }

                double res = 0;
                for (std::shared_ptr<Element>& e : p)
                    res += ((NumberToken&)*e).value;
                return std::static_pointer_cast<Element>(std::make_shared<NumberToken>(res));
            }

            boost::optional<std::shared_ptr<Element>> Subtract(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() < 2) throw WrongSignatureException();

                /* early processing */
                for (std::shared_ptr<Element>& e : p)
                    Core::Process(e);

                /* validating signature */
                for (std::shared_ptr<Element>& e : p)
                    if (e->GetElementType() != Element::Type::Token)
                        throw WrongSignatureException();

                for (std::shared_ptr<Element>& e : p)
                    e = Convert<Token::Type::Number>(std::static_pointer_cast<Token>(e), false);

                for (std::shared_ptr<Element>& e : p)
                    if (((Token&)*e).GetTokenType() != Token::Type::Number)
                        throw WrongSignatureException();

                double res = ((NumberToken&)*p[0]).value;
                for (size_t i = 1; i < p.size(); i++)
                    res -= ((NumberToken&)*p[i]).value;
                return std::static_pointer_cast<Element>(std::make_shared<NumberToken>(res));
            }

            boost::optional<std::shared_ptr<Element>> Equal(Pack& p)
            {
                /* validating arguments amount */
                if (p.size() == 0) return boost::none;
                if (p.size() != 2) throw WrongSignatureException();

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
                return std::static_pointer_cast<Element>(std::make_shared<BooleanToken>(res));
            }

            boost::optional<std::shared_ptr<Element>> NotEqual(Pack& p)
            {
                return std::static_pointer_cast<Element>(std::make_shared<BooleanToken>(!((BooleanToken&)Equal(p)).value));
            }

            boost::optional<std::shared_ptr<Element>> Greater(Pack& p) { return boost::none; }

            boost::optional<std::shared_ptr<Element>> Less(Pack& p) { return boost::none; }

            boost::optional<std::shared_ptr<Element>> GreaterOrEqual(Pack& p) { return boost::none; }

            boost::optional<std::shared_ptr<Element>> LessOrEqual(Pack& p) { return boost::none; }

            boost::optional<std::shared_ptr<Element>> Assign(Pack& p) { return boost::none; }

            boost::optional<std::shared_ptr<Element>> Exit(Pack& p) { exit(EXIT_SUCCESS); }

            boost::optional<std::shared_ptr<Element>> DoSomething(Pack& p)
            {
                std::vector<std::string> answers({
                    "Hunting unicorns...",
                    "Hacking the CIA...",
                    "Launching WannaCry...",
                    "Shorting BTC...",
                    "Saving humanity...",
                    "Creating new JS framework...",
                    "Nuking Mars..." });
                std::cout << answers[rand() % answers.size()] << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                return boost::none;
            }
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

            { "exit",         impl::Exit           },
            { "do_something", impl::DoSomething    }
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
