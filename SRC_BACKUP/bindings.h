#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <map>

namespace stx
{
    class Element;
    class Token;
    class Pack;

    class Operator
    {
    public:
        enum class Type
        {
            Unknown,
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

        Type type;
        std::string key;
        std::function<std::shared_ptr<Element>(const Element&, const Element&)> invoke;
    };

    typedef std::map<std::string, std::function<std::shared_ptr<Element>(const Element&, const Element&)>> OptMap;
    typedef std::map<std::string, std::function<void(Pack&)>> FuncMap;
    typedef std::map<std::string, std::shared_ptr<Token>> VarMap;
    typedef std::map<std::string, std::function<std::shared_ptr<Token>()>> MacroMap;

    struct Bindings
    {
        OptMap operators;
        FuncMap functions;
        VarMap variables;
        VarMap variables;
    };

    struct Function
    {
        std::string key;
        std::function<void(Pack&)> invoke;
    };

    struct Variable
    {
        std::string key;
        std::shared_ptr<Token> value;
    };

    struct Macro
    {
        std::string key;
        std::function<std::shared_ptr<Token>()> value;
    };
}