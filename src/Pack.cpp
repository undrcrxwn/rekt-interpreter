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
        return "";
    }

    void Pack::Chop(const OptMap& ots)
    {
        std::map<unsigned char, std::vector<std::string>> groups;
        for (auto& o : ots)
            groups[o.second.second].push_back(o.first);
        for (auto& group : groups)
            PackByOperators(group.second);
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

    void Pack::PackByOperators(const std::vector<std::string>& ots)
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
                    //? todo: throw unsupported signature
                    continue;
                }

                std::shared_ptr<Element> l = (*this)[i - 1];
                std::shared_ptr<Element> r = (*this)[i + 1];

                if (std::find(ots.begin(), ots.end(), t->content) == ots.end())
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
                p->PackByOperators(ots);
            }
        }
    }
}