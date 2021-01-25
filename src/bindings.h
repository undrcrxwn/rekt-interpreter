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


    typedef std::function<std::shared_ptr<Element>(std::shared_ptr<Element>, std::shared_ptr<Element>)> OptSignature;
    
    class OptInfo
    {
    public:
        OptSignature signature;
        unsigned char priority;
        bool isLazyProcessed;
    };

    typedef std::map<std::string, OptInfo> OptMap;
    typedef std::map<std::string, std::function<void(Pack&)>> FuncMap;
    typedef std::map<std::string, std::shared_ptr<Token>> VarMap;
    typedef std::map<std::string, std::function<std::shared_ptr<Element>()>> MacroMap;

    namespace defaults
    {
        namespace impl
        {
            std::shared_ptr<Element> Power(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Multiply(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Divide(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Modulo(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Add(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Subtract(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> For(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Equal(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> NotEqual(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Greater(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Less(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> GreaterOrEqual(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> LessOrEqual(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
            std::shared_ptr<Element> Assign(std::shared_ptr<Element> l, std::shared_ptr<Element> r);
        }
    }

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
