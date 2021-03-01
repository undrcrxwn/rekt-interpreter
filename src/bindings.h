#pragma once
#include <string>
#include <memory>
#include <functional>
#include <map>
#include <utility>
#include <boost/optional.hpp>

namespace rekt
{
    class Element;
    class Token;
    class Pack;

    typedef std::function<boost::optional<std::shared_ptr<Element>>(Pack&)> FuncProcessor;

    class OptInfo
    {
    public:
        FuncProcessor* processor;
        unsigned char precedence;
        bool isLazyProcessed;
    };

    typedef std::unordered_map<std::string, FuncProcessor> FuncMap;
    typedef std::unordered_map<std::string, OptInfo> OptMap;
    typedef std::unordered_map<std::string, std::shared_ptr<Token>> VarMap;
    typedef std::unordered_map<std::string, std::function<std::shared_ptr<Element>()>> MacroMap;

    struct Bindings
    {
        OptMap operators;
        FuncMap functions;
        VarMap variables;
        MacroMap macros;

        Bindings();
    };
}
