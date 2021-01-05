#include "src/Pack.h"
#include "src/Parser.h"

int main()
{
    std::string input = R"(\(\) hello \+   \ 0\  there NaN (1 2 (()) NULL (0 (1 (2 (3) 2) 1) 0) ) "1 2  3\"" fd for PI + 2)";
    std::cout << "RAW INPUT\n" << input << "\n\n\n";

    stx::Parser parser;
    stx::Pack root = parser.Parse(input);

    std::cout << "STRING OUTPUT\n";
    std::cout << root.ToString() << "\n\n\n";

    system("pause");
    return EXIT_SUCCESS;
}