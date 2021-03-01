#include "token.h"
#include "../core/core.h"
#include "pack.h"
#include "../exception.h"
#include <string>

namespace rekt
{
    std::shared_ptr<Element> MacroToken::Resolve()
    {
        std::shared_ptr<Element> resolved = (*resolver)();

        static size_t iterations = 0;
        if (iterations > MaxRecursionDepth)
            throw MaximumRecursionDepthException("Attempts: " + std::to_string(iterations) + " > " + std::to_string(MaxRecursionDepth) + ".");

        iterations++;
        Core::Resolve(resolved);
        iterations--;

        return resolved;
    }

    boost::optional<std::shared_ptr<Element>> FunctionToken::Process()
    {
        auto res = (*processor)(arguments);
        if (!res) return boost::none;
        Core::Process(res.value());
        return res;
    }

    std::shared_ptr<Element> FunctionToken::Resolve()
    {
        Core::Resolve(arguments);
        return shared_from_this();
    }

    bool StringValueBasedToken::operator==(const Token& t) const
    {
        if (GetTokenType() != t.GetTokenType())
            return false;
        return content == ((StringValueBasedToken&)t).content;
    }

    bool BooleanToken::operator==(const Token& t) const
    {
        if (GetTokenType() != t.GetTokenType())
            return false;
        return value == ((BooleanToken&)t).value;
    }

    bool NumberToken::operator==(const Token& t) const
    {
        if (GetTokenType() != t.GetTokenType())
            return false;
        return value == ((NumberToken&)t).value;
    }

    bool FunctionToken::operator==(const Token& t) const
    {
        if (GetTokenType() != t.GetTokenType())
            return false;
        return processor == ((FunctionToken&)t).processor &&
            arguments == ((FunctionToken&)t).arguments;
    }
}
