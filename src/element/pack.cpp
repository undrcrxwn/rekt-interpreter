#include "pack.h"
#include "token.h"
#include <utility>
#include <iostream>
#include <windows.h>
#include <iomanip>

namespace rekt
{
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
                os << t->content;
                SetConsoleTextAttribute(hConsole, 7);
                int offset = 25 - tab - t->content.size();
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
                            "Number",
                            "Function",
                            "String"
                        })[size_t(t->GetTokenType())];

                SetConsoleTextAttribute(hConsole, 8);
                os << this << " / " << e.get() << "   ";
                os << char(int(this) % ('A' - 'z') + 'A') << " ";
                os << char(int(e.get()) % ('A' - 'z') + 'A');
                SetConsoleTextAttribute(hConsole, 7);

                os << "\n";
            }
            else if (type == Element::Type::Pack)
                p->Print(os, tab + 1);
            else
                os << "Unsupported Element::Type.\n";
        }

        return std::make_pair(&os, tab);
    }
}