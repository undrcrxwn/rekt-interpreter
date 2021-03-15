#pragma once
#include "../bindings.h"
#include "../element/element.h"
#include "../element/pack.h"
#include "../element/token.h"
#include <memory>
#include <vector>

namespace rekt
{
    class Parser
    {
    public:
        Bindings bindings;

        Parser() = default;
        Parser(Bindings& b) : bindings(b) {};
        std::shared_ptr<Pack> Parse(const std::string& s);

    private:
        Pack SplitInWords(const std::string& s);
        Pack SplitByOperators(const std::string& s);
        std::shared_ptr<Token> Tokenize(const std::string& s);
    };
}