#pragma once
#include <string>
#include <memory>
#include <functional>
#include <map>
#include <utility>

namespace rekt
{
    class Element;
    class Token;
    class Pack;

    typedef std::function<std::shared_ptr<Element>(Pack&)> FuncProcessor;

    class OptInfo
    {
    public:
        FuncProcessor* processor;
        unsigned char precedence;
        bool isLazyProcessed;
    };

    typedef std::map<std::string, FuncProcessor> FuncMap;
    typedef std::map<std::string, OptInfo> OptMap;
    typedef std::map<std::string, std::shared_ptr<Token>> VarMap;
    typedef std::map<std::string, std::function<std::shared_ptr<Element>()>> MacroMap;

    class Bindings
    {
    public:
        OptMap operators;
        FuncMap functions;
        VarMap variables;
        MacroMap macros;

        Bindings();
    };
}
