#include "parser.h"
#include <string>

namespace rekt
{
    std::shared_ptr<Pack> Parser::Parse(const std::string& s)
    {
        return Parse(s, 0).first;
    }

    std::pair<std::shared_ptr<Pack>, size_t> Parser::Parse(const std::string& s, size_t i)
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
                if (s[i] == '"') isForcedString = !isForcedString;
                else lastContent += s[i];
                continue;
            }

            if (s[i] == ' ')
            {
                if (!lastContent.empty())
                {
                    res->push_back(ParseToken(lastContent));
                    lastContent = "";
                }
            }
            else if (s[i] == '"')
                isForcedString = !isForcedString;
            else if (s[i] == '(')
            {
                if (!lastContent.empty())
                {
                    res->push_back(ParseToken(lastContent));
                    lastContent = "";
                }

                auto [p, lastIdx] = Parse(s, i + 1);
                i = lastIdx;

                if (p->size() > 0 && res->size() > 0 &&
                    res->back()->GetElementType() == Element::Type::Token &&
                    ((Token&)*res->back()).GetTokenType() == Token::Type::Function)
                    ((FunctionToken&)*res->back()).arguments = *p;
                else
                    res->push_back(p);
            }
            else if (s[i] == ')')
            {
                if (!lastContent.empty())
                {
                    res->push_back(ParseToken(lastContent));
                    lastContent = "";
                }
                break;
            }
            else
                lastContent += s[i];
        }

        if (!lastContent.empty())
            res->push_back(ParseToken(lastContent));

        if (res->size() == 1 &&
            (*res)[0]->GetElementType() == Element::Type::Token &&
            ((Token*)((*res)[0].get()))->ToString().empty())
            res->pop_back();

        return std::make_pair(res, i);
    }

    std::shared_ptr<Token> Parser::ParseToken(const std::string& s)
    {
        // null
        if (s == "" || s == "NULL")
            return std::make_shared<NullToken>();

        // boolean
        if (s == "true")       return std::make_shared<BooleanToken>(true);
        else if (s == "false") return std::make_shared<BooleanToken>(false);

        // operator
        auto opt = bindings.operators.find(s);
        if (opt != bindings.operators.end())
            return std::make_shared<OperatorToken>(s, opt->second.processor);

        // variable
        if (s[0] == '$' && s.size() > 1)
        {
            std::string varName = s.substr(1, s.size());
            auto var = bindings.variables.find(varName);
            if (var == bindings.variables.end())
                throw std::runtime_error("Parsing exception: an undeclared variable " + s + " was used.");
            return std::make_shared<VariableToken>(varName, var->second);
        }

        // macro
        auto macro = bindings.macros.find(s);
        if (macro != bindings.macros.end())
            return std::make_shared<MacroToken>(s, &macro->second);

        // special string cases
        std::string lowercase(s);
        std::transform(s.begin(), s.end(), lowercase.begin(),
            [](char c) { return std::tolower(c); });
        if (lowercase == "inf" || lowercase == "nan")
            return std::make_shared<StringToken>(s);

        // number
        char* p;
        if (std::isinf(strtod(s.c_str(), &p)))
            throw std::runtime_error("Parsing exception: number value " + s + " is not valid.");
        if (!*p)
            return std::make_shared<NumberToken>(strtod(s.c_str(), nullptr));

        // function
        auto func = bindings.functions.find(s);
        if (func != bindings.functions.end())
            return std::make_shared<FunctionToken>(s, &func->second);

        // string
        std::string trimmed(s);
        for (size_t i = 0; i < trimmed.size(); i++)
            if (trimmed[i] == '\\') trimmed.erase(trimmed.begin() + i);
        return std::make_shared<StringToken>(trimmed);
    }
}