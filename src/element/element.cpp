#include "element.h"
#include "token.h"
#include "pack.h"

namespace rekt
{
    bool Element::operator==(const Element& e) const
    {
        Element::Type type = e.GetElementType();
        if (GetElementType() != type)
            return false;

        if (type == Element::Type::Token)
            return (Token&)*this == (Token&)e;

        if (type == Element::Type::Pack)
            return (Pack&)*this == (Pack&)e;

        throw std::runtime_error("Connot compare the elements.");
    }

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