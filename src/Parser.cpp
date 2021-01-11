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
            { "PI",   []() { return std::make_shared<NumberToken>("3.141592"); } },
            { "RAND", []() { return std::make_shared<NumberToken>(std::to_string(rand() / float(RAND_MAX))); } },
            { "NL",   []() { return std::make_shared<StringToken>("\n"); } }
        };
    }

    Parser::Parser()
    {
        Bindings b;
        b.operators = defaults::operators;
        b.macros = defaults::macros;
        *this = Parser(b);
    }

    std::shared_ptr<Pack> Parser::Parse(const std::string& s)
    {
        std::cout << "RAW PACK\n\n";
        std::shared_ptr<Pack> res = FormRawPack(s);
        res->Print() << "\n\n";

        std::cout << "CHOP\n\n";
        std::map<unsigned char, std::vector<std::string>> groups;
        for (auto& o : bindings.operators)
            groups[o.second.second].push_back(o.first);
        for (auto& group : groups)
        {
            OperatorPackingVisitor packer(group.second);
            res->Accept(packer);
        }
        res->Print() << "\n\n";

        return res;
    }

    std::shared_ptr<Pack> Parser::FormRawPack(const std::string& s)
    {
        return FormRawPack(s, 0).first;
    }

    std::pair<std::shared_ptr<Pack>, size_t> Parser::FormRawPack(const std::string& s, size_t i)
    {
        std::shared_ptr<Pack> res(new Pack);
        std::string lastContent = "";

        bool isForcedString = false;
        for (; i < s.size(); i++)
        {
            if (i > 0 && s[i - 1] == '\\')
            {
                lastContent += s[i];
                continue;
            }

            if (isForcedString)
            {
                if (s[i] == '"')
                    isForcedString = !isForcedString;
                else
                    lastContent += s[i];
                continue;
            }

            if (s[i] == ' ')
            {
                if (lastContent != "")
                {
                    res->push_back(ParseToken(lastContent));
                    lastContent = "";
                }
            }
            else if (s[i] == '"')
                isForcedString = !isForcedString;
            else if (s[i] == '(')
            {
                std::pair<std::shared_ptr<Pack>, size_t> resp = FormRawPack(s, i + 1);
                if (resp.first->size() > 0)
                    res->push_back(resp.first);
                i = resp.second;
            }
            else if (s[i] == ')')
            {
                if (lastContent != "")
                {
                    res->push_back(ParseToken(lastContent));
                    lastContent = "";
                }
                break;
            }
            else
                lastContent += s[i];
        }

        if (lastContent != "")
            res->push_back(ParseToken(lastContent));

        if (res->size() == 1 &&
            (*res)[0]->GetElementType() == Element::Type::Token &&
            ((Token*)((*res)[0].get()))->content == "")
            res->pop_back();

        return std::make_pair(res, i);
    }

    std::shared_ptr<Token> Parser::ParseToken(const std::string& s)
    {
        // null
        if (s == "" || s == "NULL")
            return std::shared_ptr<Token>(new NullToken);

        // operator
        auto opt = bindings.operators.find(s);
        if (opt != bindings.operators.end())
            return std::shared_ptr<Token>(new OperatorToken(s, &opt->second.first));

        // variable
        if (s[0] == '$' && s.size() > 1)
        {
            std::string varName = s.substr(1, s.size());
            auto var = bindings.variables.find(varName);
            if (var == bindings.variables.end())
                throw std::runtime_error("Parsing exception: an undeclared variable " + s + " was used.");
            return std::shared_ptr<Token>(new VariableToken(varName, var->second));
        }

        // macro
        auto macro = bindings.macros.find(s);
        if (macro != bindings.macros.end())
            return std::shared_ptr<Token>(new MacroToken(s, &macro->second));

        // special string cases
        std::string lowercase(s);
        std::transform(s.begin(), s.end(), lowercase.begin(),
            [](char c) { return std::tolower(c); });
        if (lowercase == "inf" || lowercase == "nan")
            return std::shared_ptr<Token>(new StringToken(s));

        // number
        char* p;
        if (std::isinf(strtod(s.c_str(), &p)))
            throw std::runtime_error("Parsing exception: number value " + s + " is not valid.");

        if (!*p)
            return std::shared_ptr<Token>(new NumberToken(s));

        // function
        auto func = bindings.functions.find(s);
        if (func != bindings.functions.end())
            return std::shared_ptr<Token>(new FunctionToken(s, &func->second));

        // string
        std::string trimmed(s);
        for (size_t i = 0; i < trimmed.size(); i++)
            if (trimmed[i] == '\\') trimmed.erase(trimmed.begin() + i);
        return std::shared_ptr<Token>(new StringToken(trimmed));
    }

    void OperatorPackingVisitor::VisitPack(Pack& p) const
    {
        for (size_t i = 0; i < p.size(); i++)
        {
            Element::Type type = p[i]->GetElementType();
            if (type == Element::Type::Token)
            {
                std::shared_ptr<Token> t = std::dynamic_pointer_cast<Token>(p[i]);
                if (t->GetTokenType() != Token::Type::Operator)
                    continue;

                if (i == 0 || i == p.size() - 1)
                    throw std::runtime_error("Choppig exception: unsupported operator signature.");

                std::shared_ptr<Element> l = p[i - 1];
                std::shared_ptr<Element> r = p[i + 1];

                if (std::find(optGroup.begin(), optGroup.end(), t->content) == optGroup.end())
                    continue;

                Pack* res = new Pack;
                res->push_back(l);
                res->push_back(t);
                res->push_back(r);
                p[i - 1] = std::shared_ptr<Element>(res);
                p.erase(p.begin() + i, p.begin() + i + 2);
                i--;
            }
            else if (type == Element::Type::Pack)
                p[i]->Accept(*(Visitor*)this);
        }
    }
}