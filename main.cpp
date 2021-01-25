#include "src/element/token.h"
#include "src/element/pack.h"
#include "src/parser/parser.h"
#include "src/core/core.h"
#include "src/bindings.h"
#include <windows.h>

int main()
{
    srand(time(0));

    std::string input = R"(\(\) hello $ $world \+   \ 0\  NaN (1 2.00 ((())) NULL (0 (1 (2 (3) 2) 1 (2 ) 1 ) 0 ) ) "1 2  3\"" fd + 2 for PI ^ 1)";
    input = "TEST for 1";
    std::cout << "RAW INPUT\n" << input << "\n\n\n";

    rekt::Parser parser;
    parser.bindings.variables.emplace("world", std::shared_ptr<rekt::Token>(new rekt::StringToken("w0rLD")));

    std::string s = "+";
    auto opt = parser.bindings.operators.find(s);
    std::string f = "PI";
    auto macro = parser.bindings.macros.find(f);

    std::function<std::shared_ptr<rekt::Element>()> test;
    test = [&]()->std::shared_ptr<rekt::Element> {
        std::shared_ptr<rekt::Pack> p(new rekt::Pack);
        p->push_back(std::shared_ptr<rekt::Token>(new rekt::MacroToken("TEST", &test)));
        p->push_back(std::shared_ptr<rekt::Token>(new rekt::OperatorToken(s, &opt->second.signature, opt->second.isLazyProcessed)));
        p->push_back(std::shared_ptr<rekt::Token>(new rekt::MacroToken(f, &macro->second)));

        return p;
    };
    parser.bindings.macros.emplace("TEST", test);

    try
    {
        std::shared_ptr<rekt::Pack> root = parser.Parse(input);

        std::cout << "STRING OUTPUT\n";
        std::cout << root->ToString() << "\n\n\n";

        rekt::Core core(parser.bindings);
        core.Execute(root);

        std::cout << "PROCESSED OUTPUT\n";
        root->Print() << "\n\n\n";
    }
    catch (const std::exception& exc)
    {
        HANDLE hConsole = GetStdHandle(-11);
        SetConsoleTextAttribute(hConsole, 64);
        std::cout << " REKT exception thrown: " << exc.what() << " \n";
        SetConsoleTextAttribute(hConsole, 7);
    }

    system("pause");
    return EXIT_SUCCESS;
}