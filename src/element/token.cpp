#include "token.h"
#include "../core/core.h"
#include "pack.h"

namespace rekt
{
    std::shared_ptr<Element> OperatorToken::Process(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
    {
        Pack args = { l, r };
        return (*processor)(args);
    }

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
        std::shared_ptr<Element> res = (*processor)(arguments);
        Core::Process(res);
        return res;
    }

    std::shared_ptr<Element> FunctionToken::Resolve()
    {
        Core::Resolve(arguments);
        return shared_from_this();
    }

    std::string Token::ToString() const
    {
        return content;
    }
}