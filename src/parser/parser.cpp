#include "parser.h"
#include <string>
#include <boost/static_string.hpp>
#include <boost/algorithm/string/replace.hpp>

namespace rekt
{
    Pack Parser::SplitByOperators(const std::string& s)
    {
        Pack res;
        for (auto [optContent, optInfo] : bindings.operators)
        {
            size_t found = s.find(optContent);
            if (found == s.npos) continue;

            std::string left = s.substr(0, found);
            std::string mid = s.substr(found, optContent.length());
            std::string right = s.substr(found + optContent.length(), std::string::npos);

            Pack split = SplitByOperators(left);
            res.insert(res.end(), split.begin(), split.end());
            res.push_back(std::make_shared<UndefinedToken>(mid));
            split = SplitByOperators(right);
            res.insert(res.end(), split.begin(), split.end());
            break;
        }

        if (!res.empty())   return res;
        if (!s.empty())     return { std::make_shared<UndefinedToken>(s) };
        else                return {};
    }

    Pack Parser::SplitInWords(const std::string& s)
    {
        Pack words;
        std::shared_ptr<StringValueBasedToken> last(new UndefinedToken);
        for (size_t i = 0; i < s.size(); i++)
        {
            if (last->GetTokenType() == Token::Type::String)
            {
                if (i > 0 && i < s.size() - 1 && s[i] == '\\')
                {
                    switch (s[i + 1])
                    {
                    case 'r':
                        last->content += '\r';
                        break;
                    case 'n':
                        last->content += '\n';
                        break;
                    case 't':
                        last->content += '\t';
                        break;
                    default:
                        last->content += s[i + 1];
                        break;
                    }

                    i++;
                    continue;
                }

                if (s[i] == '"')
                {
                    words.push_back(last->Clone());
                    last = std::make_shared<UndefinedToken>("");
                }
                else
                    last->content += s[i];
            }
            else if (s[i] == '"')
            {
                if (!last->content.empty())
                    words.push_back(last->Clone());
                last = std::make_shared<StringToken>("");
            }
            else if (s[i] == '(' || s[i] == ')')
            {
                if (!last->content.empty())
                {
                    words.push_back(last->Clone());
                    last->content = "";
                }
                words.push_back(std::make_shared<UndefinedToken>(std::string(1, s[i])));
            }
            else if (s[i] == ' ')
            {
                if (!last->content.empty())
                {
                    words.push_back(last->Clone());
                    last->content = "";
                }
            }
            else
                last->content += s[i];
        }

        if (!last->content.empty())
            words.push_back(last);

        return words;
    }

    std::shared_ptr<Pack> Parser::Parse(const std::string& s)
    {
        std::shared_ptr<Pack> res(new Pack);
        Pack words = SplitInWords(s);

        std::cout << "\nSPLIT IN WORDS\n";
        words.Print() << "\n";

        for (auto& word : words)
        {
            Pack split = { std::static_pointer_cast<Token>(word) };
            if (((Token&)*word).GetTokenType() == Token::Type::Undefined)
                split = SplitByOperators(((StringValueBasedToken&)*word).content);

            for (auto& e : split)
            {
                std::shared_ptr<Token> t = std::static_pointer_cast<Token>(e);
                if (t->GetTokenType() == Token::Type::Undefined)
                    t = Tokenize(((StringValueBasedToken&)*t).content);
                res->push_back(t);
            }
        }

        return res;
    }

    std::shared_ptr<Token> Parser::Tokenize(const std::string& s)
    {
        // null
        if (s == "" || s == "NULL")
            return std::make_shared<NullToken>();

        // bracket
        if (s.size() == 1)
        {
            try
            {
                return std::make_shared<BracketToken>(s[0]);
            }
            catch (...) {}
        }

        // boolean
        if (s == "true")       return std::make_shared<BooleanToken>(true);
        else if (s == "false") return std::make_shared<BooleanToken>(false);

        // operator
        for (auto& [opt, info] : bindings.operators)
            if (s == opt) return std::make_shared<OperatorToken>(s, info.processor);

        // variable
        if (s[0] == '$' && s.size() > 1)
        {
            std::string varName = s.substr(1, s.size());
            auto var = bindings.variables.find(varName);
            if (var == bindings.variables.end())
                throw UndeclaredVariableException(s);
            return std::make_shared<VariableToken>(varName, var->second);
        }

        // macro
        auto macro = bindings.macros.find(s);
        if (macro != bindings.macros.end())
            return std::make_shared<MacroToken>(s, &macro->second);

        // number
        char* p;
        double value = strtod(s.c_str(), &p);
        if (std::isinf(value))
            return std::make_shared<UndefinedToken>();
        if (!*p)
            return std::make_shared<NumberToken>(value);

        // function
        auto func = bindings.functions.find(s);
        if (func != bindings.functions.end())
            return std::make_shared<FunctionToken>(s, &func->second);

        // undefined
        return std::make_shared<UndefinedToken>(s, std::make_shared<Error>(Error::Type::UndefinedToken));
    }
}
