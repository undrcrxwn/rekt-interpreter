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

    std::pair<std::ostream*, size_t> Pack::Print(std::ostream& os, size_t tab) const
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        for (std::shared_ptr<Element> e : *this)
        {
            Token* t = (Token*)(e.get());
            Pack* p = (Pack*)(e.get());
            switch (e->GetElementType())
            {
            case Element::Type::Token:
                for (size_t i = 0; i < tab; i++)
                    os << " ";

                SetConsoleTextAttribute(hConsole, 112);
                os << t->content;
                SetConsoleTextAttribute(hConsole, 7);
                os << "\t" <<
                    std::vector<std::string>({
                            "Null",
                            "Operator",
                            "Variable",
                            "Macro",
                            "Number",
                            "Function",
                            "String"
                        })[size_t(t->GetTokenType())];

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
}