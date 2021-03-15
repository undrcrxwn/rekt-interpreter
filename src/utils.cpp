#include "utils.h"
#include "element/token.h"

namespace rekt
{
    namespace utils
    {
        std::string ToString(double d)
        {
            std::string res = std::to_string(d);
            res.erase(0, res.find_first_not_of('0'));
            if (res.find_first_of('.') != std::string::npos)
                res.erase(res.find_last_not_of('0') + 1, std::string::npos);
            if (res == "")
                res = "0";
            else
            {
                if (res.front() == '.')
                    res.insert(res.begin(), '0');
                if (res.back() == '.')
                    res.pop_back();
            }
            return res;
        }

        std::string AddFuncNameDecorations(std::string& s)
        {
            return "operator_" + s;
        }

        std::string RemoveFuncNameDecorations(std::string& s)
        {
            if (s.rfind("operator_", 0) == 0)
                return s.substr(9, s.size() - 8);
            return s;
        }

        std::string GetElementTypeName(Element& e)
        {
            return std::vector<std::string>({
                "Token",
                "Pack" })[(size_t)e.GetElementType()];
        }

        std::string GetTokenTypeName(Token& t)
        {
            return std::vector<std::string>({
                "Null",
                "Undefined",
                "Operator",
                "Variable",
                "Macro",
                "Boolean",
                "Number",
                "Function",
                "String",
                "Bracket" })[(size_t)t.GetTokenType()];
        }
    }
}
