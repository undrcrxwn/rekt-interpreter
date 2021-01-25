#include "token.h"
#include "../core/core.h"

namespace rekt
{
    std::shared_ptr<Element> OperatorToken::Process(std::shared_ptr<Element> l, std::shared_ptr<Element> r)
    {
        return (*processor)(l, r);
    }

    std::shared_ptr<Element> MacroToken::Resolve()
    {
        std::shared_ptr<Element> resolved = (*resolver)();

        static size_t iterations = 0;
        if (iterations >= MaxRecursionDepth)
            throw std::runtime_error("Maximum recursion depth exceeded.");

        iterations++;
        ResolvingVisitor v;
        resolved->Accept(&v);
        iterations--;

        return resolved;
    }

    std::string Token::ToString() const
    {
        return content;
    }
}