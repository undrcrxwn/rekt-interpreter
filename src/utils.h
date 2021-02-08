#pragma once
#include <string>

namespace rekt
{
    namespace utils
    {
        std::string ToString(double d);
        std::string AddFuncNameDecorations(std::string& s);
        std::string RemoveFuncNameDecorations(std::string& s);
    }
}