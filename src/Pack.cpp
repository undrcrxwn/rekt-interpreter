#include "Pack.h"
#include "Token.h"
#include <utility>
#include "windows.h"

namespace stx
{
    std::ostream& Pack::Print(std::ostream& os) const
    {
        return *Print(os, 0).first;
    }

    Pack Pack::Parse(const std::string& s)
    {
        Pack result = FormRawPack(s);

        std::cout << "EXPOUND\n\n";
        result.Expound();
        result.Print() << "\n\n";

        std::cout << "NORMALIZE\n\n";
        result.Normalize();
        result.Print() << "\n\n";

        std::cout << "CHOP\n\n";
        result.Chop();
        result.Print() << "\n\n";

        std::cout << "PROCESS\n\n";
        result.Process();
        result.Print() << "\n\n";

        std::cout << "RESOLVE\n\n";
        result.Resolve();
        result.Print() << "\n\n";

        return result;
    }

    std::string Pack::ToString() const
    {
        return "";
    }

    void Pack::Expound() const
    {

    }

    void Pack::Normalize()
    {

    }

    void Pack::Resolve()
    {

    }

    void Pack::Chop()
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

    Pack Pack::FormRawPack(const std::string& s)
    {
        return FormRawPack(s, 0).first;
    }

    std::pair<Pack, size_t> Pack::FormRawPack(const std::string& s, size_t i)
    {
        Pack result;
        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));

        bool isForcedString = false;
        for (; i < s.size(); i++)
        {
            Element* last = result.back().get();

            if (i > 0 && s[i - 1] == '\\')
                ((Token*)last)->content += s[i];
            else
            {
                if (isForcedString)
                {
                    if (s[i] == '"')
                        isForcedString = !isForcedString;
                    else
                        ((Token*)last)->content += s[i];
                    continue;
                }

                if (s[i] == ' ')
                {
                    if (last->GetType() != Element::Type::Token ||
                        ((Token*)last)->content != "")
                        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));
                }
                else if (s[i] == '"')
                    isForcedString = !isForcedString;
                else if (s[i] == '(')
                {
                    if (result.back()->GetType() == Element::Type::Token &&
                        ((Token*)last)->content == "")
                        result.pop_back();
                    std::pair<Pack, size_t> resp = FormRawPack(s, i + 1);
                    result.push_back(std::make_shared<Pack>(resp.first));
                    i = resp.second;
                }
                else if (s[i] == ')')
                {
                    if (last->GetType() == Element::Type::Token &&
                        ((Token*)last)->content == "")
                        result.pop_back();
                    break;
                }
                else
                {
                    if (result.back()->GetType() != Element::Type::Token)
                        result.push_back(std::make_shared<Token>(Token::Type::Unknown, ""));
                    ((Token*)result.back().get())->content += s[i];
                }
            }
        }

        if (result.size() == 1 &&
            result[0]->GetType() == Element::Type::Token &&
            ((Token*)(result[0].get()))->content == "")
            result.pop_back();
        return std::make_pair(result, i);
    }

    void Pack::PackByOperators(std::vector<Operator::Type> opts)
    {

    }
}