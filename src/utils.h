#pragma once
#include <string>

namespace rekt
{
    class Token;

    namespace utils
    {
        std::string ToString(double d);
        std::string AddFuncNameDecorations(std::string& s);
        std::string RemoveFuncNameDecorations(std::string& s);
        std::string GetTokenTypeName(Token& t);
    }
}
