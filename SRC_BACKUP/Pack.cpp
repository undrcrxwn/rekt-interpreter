#include "Pack.h"
#include "Token.h"
#include <utility>
#include "windows.h"
#include <iostream>

namespace stx
{
    std::ostream& Pack::Print(std::ostream& os) const
    {
        return *Print(os, 0).first;
    }

    std::string Pack::ToString() const
    {
        //? todo: Normalize();
        //?       return content;
        return "";
    }

    void Pack::Chop(const Bindings& b)
    {
        // for
        PackByOperators(b,
            { Operator::Type::For });

        // ^
        PackByOperators(b,
            { Operator::Type::Power });

        // * /
        PackByOperators(b,
            { Operator::Type::Multiply,
              Operator::Type::Divide });

        // + -
        PackByOperators(b,
            { Operator::Type::Add,
              Operator::Type::Subtract });

        // == > < >= <=
        PackByOperators(b,
            { Operator::Type::Equal,
              Operator::Type::Greater,
              Operator::Type::Less,
              Operator::Type::GreaterOrEqual,
              Operator::Type::LessOrEqual });
    }

    void Pack::Resolve()
    {

    }

    void Pack::Process()
    {

    }

    std::pair<std::ostream*, size_t> Pack::Print(std::ostream& os, size_t tab) const
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        for (std::shared_ptr<Element> e : *this)
        {
            Token* t = (Token*)(e.get());
            Pack* p = (Pack*)(e.get());
            switch (e->GetType())
            {
            case Element::Type::Token:
                for (size_t i = 0; i < tab; i++)
                    os << " ";

                SetConsoleTextAttribute(hConsole, 112);
                os << t->content;
                SetConsoleTextAttribute(hConsole, 7);
                os << "\t" <<
                    std::vector<std::string>({
                            "Unknown",
                            "Function",
                            "Number",
                            "Null",
                            "Variable",
                            "Macro",
                            "String",
                            "Operator"
                        })[size_t(t->type)];

                if (false /*?todo: error statement*/)
                    os << " <E>";

                os << "\n";
                break;
            case Element::Type::Pack:
                p->Print(os, tab + 1);
                break;
            default:
                os << "Unsupported Element::Type.\n";
                break;
            }
        }

        return std::make_pair(&os, tab);
    }

    void Pack::PackByOperators(const Bindings& b, std::vector<Operator::Type> ots)
    {
        for (size_t i = 0; i < size(); i++)
        {
            Element::Type type = (*this)[i]->GetType();
            if (type == Element::Type::Token)
            {
                std::shared_ptr<Token> t = std::dynamic_pointer_cast<Token>((*this)[i]);
                if (t->type != Token::Type::Operator)
                    continue;

                if (i == 0 || i == size() - 1)
                {
                    //? todo: t -> set error: unsupported signature
                    continue;
                }

                std::shared_ptr<Element> l = (*this)[i - 1];
                std::shared_ptr<Element> r = (*this)[i + 1];

                Operator::Type myOt = std::find_if(b.operators.begin(), b.operators.end(), [&](const Operator& o)
                    { return o.key == t->content; })->type;
                bool found = std::find_if(ots.begin(), ots.end(), [&](const Operator::Type& ot)
                    { return ot == myOt; }) != ots.end();
                if (!found)
                    continue;

                Pack* p = new Pack;
                p->push_back(l);
                p->push_back(t);
                p->push_back(r);
                (*this)[i - 1] = std::shared_ptr<Element>(p);
                erase(begin() + i, begin() + i + 2);
                i--;
            }
            else if (type == Element::Type::Pack)
            {
                Pack* p = (Pack*)(*this)[i].get();
                p->PackByOperators(b, ots);
            }
        }
    }
}