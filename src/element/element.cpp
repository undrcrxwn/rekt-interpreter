#include "element.h"
#include "pack.h"

namespace rekt
{
    std::shared_ptr<Element> Element::GetDeepest()
    {
        std::shared_ptr<Element> res = shared_from_this();
        for (size_t attempts = 250; attempts > 0; attempts--)
        {
            if (res->GetElementType() == Element::Type::Token)
                return res;
            else if (res->GetElementType() == Element::Type::Pack)
            {
                std::shared_ptr<Pack> p = std::dynamic_pointer_cast<Pack>(res);
                if (p->size() == 1)
                    res = (*p)[0];
                else
                    return res;
            }
        }

        throw std::runtime_error("Cannot get the deepest element: ran out of attempts.");
    }
}