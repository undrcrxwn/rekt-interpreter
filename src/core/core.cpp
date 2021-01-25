#include "core.h"
#include "../element/token.h"
#include "../element/pack.h"

namespace rekt
{
    void Core::Execute(std::shared_ptr<Pack>& p)
    {
        Process(p);
        Resolve(p);
    }

    void Core::Process(std::shared_ptr<Pack>& p)
    {
        ProcessingVisitor v;
        p->Accept(&v);
        std::cout << "PROCESSING\n";
        p->Print() << "\n";
    }

    void Core::Resolve(std::shared_ptr<Pack>& p)
    {
        ResolvingVisitor resolver;
        resolver.Visit(std::move(p));
    }

    void ProcessingVisitor::Visit(std::shared_ptr<Pack>&& p) const
    {
        if (p->size() != 3 ||
            (*p)[1]->GetElementType() != Element::Type::Token ||
            ((Token&)(*(*p)[1])).GetTokenType() != Token::Type::Operator)
        {
            for (std::shared_ptr<Element> e : *p)
                e->Accept((Element::Visitor*)this);
            return;
        }

        std::shared_ptr<OperatorToken> opt = std::dynamic_pointer_cast<OperatorToken>((*p)[1]);
        std::shared_ptr<Element>& l = (*p)[0];
        std::shared_ptr<Element>& r = (*p)[2];

        if (!opt->isLazyProcessed)
        {
            l->Accept((Element::Visitor*)this);
            if (l->GetElementType() == Element::Type::Pack)
                l->Accept((Element::Visitor*)this);
            l = l->GetDeepest();
        }

        r->Accept((Element::Visitor*)this);
        if ((*p)[2]->GetElementType() == Element::Type::Pack)
            r->Accept((Element::Visitor*)this);
        r = r->GetDeepest();

        std::shared_ptr<Element> res = opt->Process((*p)[0], (*p)[2]);
        p->clear();
        p->push_back(res);
    }

    void ResolvingVisitor::Visit(std::shared_ptr<Pack>&& p) const
    {
        for (std::shared_ptr<Element>& e : *p)
        {
            if (e->GetElementType() == Element::Type::Token)
                e = std::dynamic_pointer_cast<Token>(e)->Resolve();
            else
                e->Accept((Element::Visitor*)this);
        }
    }
}