#include "src/Token.h"
#include "src/Pack.h"
#include "src/Parser.h"
#include "windows.h"

int main()
{
    std::string input = R"(\(\) hello $ $world \+   \ 0\  NaN (1 2 ((())) NULL (0 (1 (2 (3) 2) 1 (2 ) 1 ) 0 ) ) "1 2  3\"" fd + 2 for PI ^ 1)";
    std::cout << "RAW INPUT\n" << input << "\n\n\n";

    stx::Parser parser;
    parser.bindings.variables.emplace("world", std::shared_ptr<stx::Token>(new stx::StringToken("w0rLD")));
    std::shared_ptr<stx::Pack> root = parser.Parse(input);
    
    std::cout << "STRING OUTPUT\n";
    std::cout << root->ToString() << "\n\n\n";

    system("pause");
    return EXIT_SUCCESS;
}