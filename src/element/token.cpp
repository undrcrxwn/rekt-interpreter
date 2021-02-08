#include "token.h"
#include "../core/core.h"
#include "pack.h"

namespace rekt
{
    std::shared_ptr<Element> MacroToken::Resolve()
    {
        std::shared_ptr<Element> resolved = (*resolver)();

        static size_t iterations = 0;
        if (iterations >= MaxRecursionDepth)
            throw std::runtime_error("Maximum recursion depth exceeded.");

        iterations++;
        Core::Resolve(resolved);
        iterations--;

        return resolved;
    }

    std::shared_ptr<Element> FunctionToken::Process()
    {
        if (arguments.size() == 0)
            return shared_from_this();

        std::shared_ptr<Element> res = (*processor)(arguments);
        Core::Process(res);
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