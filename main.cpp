#include "src/Token.h"
#include "src/Pack.h"
#include <iostream>

int main()
{
    std::string input = R"(\(\) hello   \ 0\  there (1 2 (()) NULL (0 (1 (2 (3) 2) 1) 0) ) "1 2  3\"" fd)";

    std::cout << "RAW INPUT\n" << input << "\n\n\n";
    stx::Pack root = stx::Pack::Parse(input);

    system("pause");
    return EXIT_SUCCESS;
}