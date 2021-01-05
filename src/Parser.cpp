#include "Parser.h"
#include "Token.h"
#include "Pack.h"
#include "bindings.h"
#include <string>

namespace stx
{
    namespace defaults
    {
        namespace impl
        {
            std::shared_ptr<Element> Power(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Multiply(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Divide(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Modulo(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Add(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Subtract(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> For(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Equal(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> NotEqual(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Greater(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Less(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> GreaterOrEqual(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> LessOrEqual(const Element& l, const Element& r) { return nullptr; }
            std::shared_ptr<Element> Assign(const Element& l, const Element& r) { return nullptr; }
        }

        const OptMap operators = {
            { "for", { impl::For,            0 } },                                             
            { "^",   { impl::Power,          1 } },            
            { "*",   { impl::Multiply,       2 } },
            { "/",   { impl::Divide,         2 } },
            { "%",   { impl::Modulo,         2 } },
            { "+",   { impl::Add,            3 } },
            { "-",   { impl::Subtract,       3 } },
            { "==",  { impl::Equal,          4 } },
            { "!=",  { impl::NotEqual,       4 } },
            { ">",   { impl::Greater,        4 } },
            { "<",   { impl::Less,           4 } },
            { ">=",  { impl::GreaterOrEqual, 4 } },
            { "<=",  { impl::LessOrEqual,    4 } },                                
            { "=",   { impl::Assign,         5 } }
        };

        const MacroMap macros = {
            { "PI",   []() { return std::make_shared<Token>(Token(Token::Type::Number, "3.141592")); } },
            { "RAND", []() { return std::make_shared<Token>(Token(Token::Type::Number, std::to_string(rand() / float(RAND_MAX)))); } },
            { "NL",   []() { return std::make_shared<Token>(Token(Token::Type::String, "\n")); } }
        };
    }

    Parser::Parser()
    {
        Bindings b;
        b.operators = defaults::operators;
        b.macros = defaults::macros;
        *this = Parser(b);
    }

    Pack Parser::Parse(const std::string& s) const
    {
        Pack result;

        std::cout << "RAW PACK\n\n";
        result = FormRawPack(s);
        result.Print() << "\n\n";

        std::cout << "EXPOUND\n\n";
        result.Expound(bindings);
        result.Print() << "\n\n";

        std::cout << "NORMALIZE\n\n";
        result.Normalize();
        result.Print() << "\n\n";

        std::cout << "CHOP\n\n";
        result.Chop(bindings.operators);
        result.Print() << "\n\n";

        std::cout << "PROCESS\n\n";
        result.Process();
        result.Print() << "\n\n";

        std::cout << "RESOLVE\n\n";
        result.Resolve();
        result.Print() << "\n\n";

        return result;
    }

    Pack Parser::FormRawPack(const std::string& s)
    {
        return FormRawPack(s, 0).first;
    }

    std::pair<Pack, size_t> Parser::FormRawPack(const std::string& s, size_t i)
    {
        Pack result;
        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));

        bool isForcedString = false;
        for (; i < s.size(); i++)
        {
            Element* last = result.back().get();

            if (i > 0 && s[i - 1] == '\\')
                ((Token*)last)->content += s[i];
            else
            {
                if (isForcedString)
                {
                    if (s[i] == '"')
                        isForcedString = !isForcedString;
                    else
                        ((Token*)last)->content += s[i];
                    continue;
                }

                if (s[i] == ' ')
                {
                    if (last->GetType() != Element::Type::Token ||
                        ((Token*)last)->content != "")
                        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));
                }
                else if (s[i] == '"')
                    isForcedString = !isForcedString;
                else if (s[i] == '(')
                {
                    if (result.back()->GetType() == Element::Type::Token &&
                        ((Token*)last)->content == "")
                        result.pop_back();
                    std::pair<Pack, size_t> resp = FormRawPack(s, i + 1);
                    if (resp.first.size() == 0)
                        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));
                    else
                        result.push_back(std::make_shared<Pack>(resp.first));
                    i = resp.second;
                }
                else if (s[i] == ')')
                {
                    if (last->GetType() == Element::Type::Token &&
                        ((Token*)last)->content == "")
                        result.pop_back();
                    break;
                }
                else
                {
                    if (result.back()->GetType() != Element::Type::Token)
                        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));
                    ((Token*)result.back().get())->content += s[i];
                }
            }
        }

        if (result.size() == 1 &&
            result[0]->GetType() == Element::Type::Token &&
            ((Token*)(result[0].get()))->content == "")
            result.pop_back();
        return std::make_pair(result, i);
    }
}