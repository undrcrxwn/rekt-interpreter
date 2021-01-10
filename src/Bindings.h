#pragma once
#include <string>
#include <memory>
#include <functional>
#include <map>
#include <utility>

namespace stx
{
    class Element;
    class Token;
    class Pack;

    typedef std::function<std::shared_ptr<Element>(const Element&, const Element&)> OptSignature;
    typedef std::pair<OptSignature, unsigned char> OptInfo;
    typedef std::map<std::string, OptInfo> OptMap;
    typedef std::map<std::string, std::function<void(Pack&)>> FuncMap;
    typedef std::map<std::string, std::shared_ptr<Token>> VarMap;
    typedef std::map<std::string, std::function<std::shared_ptr<Token>()>> MacroMap;

    struct Bindings
    {
        OptMap operators;
        FuncMap functions;
        VarMap variables;
        MacroMap macros;
    };
}