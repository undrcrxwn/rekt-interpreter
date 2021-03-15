#pragma once
#include <string>
#include <memory>
#include <functional>
#include <map>
#include <utility>
#include <boost/optional.hpp>
#include <string>
#include <optional>

namespace rekt
{
    class Element;
    class Token;
    class Pack;

    typedef std::function<std::optional<std::shared_ptr<Element>>(Pack&)> FuncProcessor;

    class OptInfo
    {
    public:
        FuncProcessor* processor;
        unsigned char precedence;
        bool isLazyProcessed;
    };

    struct cmpByLength
    {
        bool operator()(const std::string& l, const std::string& r) const
        {
            if (l == r) return false;
            return l.size() > r.size();
        }
    };

    typedef std::unordered_map<std::string, FuncProcessor> FuncMap;
    typedef std::multimap<std::string, OptInfo, cmpByLength> OptMap;
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
