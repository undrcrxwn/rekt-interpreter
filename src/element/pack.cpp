#include "pack.h"
#include "token.h"
#include <utility>
#include <iostream>
#include <windows.h>
#include <iomanip>

namespace rekt
{
    bool Pack::operator==(const Pack& p) const
    {
        if (size() == 0)
            return p.size() == 0;

        if (size() != p.size())
            return false;

        for (size_t i = 0; i < size(); i++)
        {
            if (*(*this)[i] != *p[i])
                return false;
        }
        return true;
    }

    std::shared_ptr<Element> Pack::Clone()
    {
        std::shared_ptr<Pack> res(new Pack);
        for (std::shared_ptr<Element>& e : *this)
            res->push_back(e->Clone());
        return res;
    }

    std::ostream& Pack::Print(std::ostream& os) const
    {
        return *Print(os, 0).first;
    }

    std::string Pack::ToString() const
    {
        std::string content;
        for (size_t i = 0; i < size(); i++)
        {
            content += (*this)[i]->ToString();
            if (i < size() - 1)
                content += " ";
        }

        if (size() == 1)
            return content;
        else
            return "(" + content + ")";
    }

    std::pair<std::ostream*, size_t> Pack::Print(std::ostream& os, size_t tab) const
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        for (std::shared_ptr<Element> e : *this)
        {
            Token* t = (Token*)(e.get());
            Pack* p = (Pack*)(e.get());

            if (!e)
            {
                for (size_t i = 0; i < tab; i++)
                    os << " ";
                os << "null/empty" << "\n";
                continue;
            }

            Element::Type type = e->GetElementType();
            if (type == Element::Type::Token)
            {
                for (size_t i = 0; i < tab; i++)
                    os << " ";

                SetConsoleTextAttribute(hConsole, 112);
                os << t->ToString();
                SetConsoleTextAttribute(hConsole, 7);

                int offset = 25 - tab - t->ToString().size();
                if (offset > 0)
                    os << std::setw(offset) << " ";
                else
                    os << "\t";

                os << std::setw(15) << std::left <<
                    std::vector<std::string>({
                            "Null",
                            "Operator",
                            "Variable",
                            "Macro",
                            "Boolean",
                            "Number",
                            "Function",
                            "String"
                        })[size_t(t->GetTokenType())];

                SetConsoleTextAttribute(hConsole, 8);

                os << this << " / " << e.get() << "   ";
                os << char(std::abs(int(this))    % ('A' - 'z') + 'A') << " ";
                os << char(std::abs(int(e.get())) % ('A' - 'z') + 'A');
                SetConsoleTextAttribute(hConsole, 7);

                os << "\n";

                if (t->GetTokenType() == Token::Type::Function)
                {
                    FunctionToken& ft = (FunctionToken&)*t;
                    ft.arguments.Print(os, tab + 1);
                }
            }
            else if (type == Element::Type::Pack)
                p->Print(os, tab + 1);
            else
                os << "Unsupported Element::Type.\n";
        }

        return std::make_pair(&os, tab);
    }
}