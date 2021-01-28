#include "core.h"
#include "../element/token.h"
#include "../element/pack.h"

namespace rekt
{
    void Core::Execute(Pack& p)
    {
        PackOperators(p);
        Process(p);
        Resolve(p);
    }

    void Core::PackOperators(Element& e)
    {
        std::map<unsigned char, std::vector<std::string>> groups;
        for (auto& o : bindings.operators)
            groups[o.second.precedence].push_back(o.first);
        for (auto& g : groups)
        {
            OperatorPacker op(g.second);
            e.Accept(&op);
        }
    }

    void OperatorPacker::Visit(Token& t)
    {
        if (t.GetTokenType() == Token::Type::Function)
            ((FunctionToken&)t).arguments.Accept(this);
    }

    void OperatorPacker::Visit(Pack& p)
    {
        for (size_t i = 0; i < p.size(); i++)
        {
            Element::Type type = p[i]->GetElementType();
            if (type == Element::Type::Token)
            {
                Token& t = (Token&)*p[i];
                if (t.GetTokenType() == Token::Type::Function)
                {
                    ((FunctionToken&)t).arguments.Accept(this);
                    continue;
                }

                if (t.GetTokenType() != Token::Type::Operator)
                    continue;

                if (i == 0 || i == p.size() - 1)
                    throw std::runtime_error("Choppig exception: unsupported operator signature.");

                if (std::find(optGroup.begin(), optGroup.end(), t.content) == optGroup.end())
                    continue;

                p[i - 1]->Accept(this);
                p[i + 1]->Accept(this);

                OperatorToken& opt = (OperatorToken&)t;
                Pack args = { p[i - 1], p[i + 1] };
                p[i - 1] = std::shared_ptr<Element>(new FunctionToken("operator_" + t.content, opt.processor, args));

                p.erase(p.begin() + i, p.begin() + i + 2);
                std::cout << "OPT PACKED\n";
                p.Print() << "\n\n";
                i--;
            }
            else
                p[i]->Accept(this);
        }
    }

    void Core::Process(std::shared_ptr<Element>& e)
    {
        Element::Type type = e->GetElementType();
        if (type == Element::Type::Token)
            e = ((Token&)*e).Process();
        else if (type == Element::Type::Pack)
            Process((Pack&)*e);

        e = e->GetDeepest();
        Resolve(e);
    }

    void Core::Process(Pack& p)
    {
        for (std::shared_ptr<Element>& e : p)
        {
            if (e->GetElementType() == Element::Type::Token)
                e = ((Token&)*e).Process();
            else
                Process(e);
        }

        std::cout << "CORE::PROCESS\n";
        p.Print() << "\n";
    }

    void Core::Resolve(std::shared_ptr<Element>& e)
    {
        Element::Type type = e->GetElementType();
        if (type == Element::Type::Token)
            e = ((Token&)*e).Resolve();
        else if (type == Element::Type::Pack)
            Resolve((Pack&)*e);
    }

    void Core::Resolve(Pack& p)
    {
        for (std::shared_ptr<Element>& e : p)
        {
            if (e->GetElementType() == Element::Type::Token)
                e = ((Token&)*e).Resolve();
            else
                Resolve(e);
        }
    }
}