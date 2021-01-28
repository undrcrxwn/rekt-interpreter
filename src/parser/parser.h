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
        std::pair<std::shared_ptr<Pack>, size_t> Parse(const std::string& s, size_t i);
        std::shared_ptr<Token> ParseToken(const std::string& s);
    };
}