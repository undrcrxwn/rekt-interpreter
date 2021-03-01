#include "src/element/token.h"
#include "src/element/pack.h"
#include "src/parser/parser.h"
#include "src/core/core.h"
#include "src/bindings.h"   

int main()
{
    srand(time(0));

    std::string input = R"(\(\) hello $ $world \+   \ 0\  NaN (1 2.00 ((())) NULL (0 (1 (2 (3) 2) 1 (2 ) 1 ) 0 ) ) "1 2  3\"" (TIME + 2) for (PI + 1) operator_for(TIME 10))";
    input = R"((operator_/ == operator_/) * 10)";
    input = R"(4 + (1 + 1) * 10)";
    /*input = "do_something for 3 "
            "2 ^ 3 "
            "6 * 7 "
            "32 / 8 "
            "23.5 % 23 "
            "xyz + 123 "
            "75 - 25 "
            "2 == 3 "
            "4 != 4 ";*/
    std::cout << "RAW INPUT:\t" << input << "\n";

    rekt::Parser parser;
    parser.bindings.variables.emplace("world", std::make_shared<rekt::StringToken>("hello world!"));

    std::shared_ptr<rekt::Pack> root = parser.Parse(input);
    std::cout << "PARSED INPUT:\t" << root->ToString() << "\n\n\n";

    std::cout << "PARSED OUTPUT\n";
    root->Print() << "\n";

    rekt::Core core(parser.bindings);
    core.Execute(*root);
    std::cout << "PROCESSED OUTPUT\n";
    root->Print() << "\n\n\n";

    system("pause");
    return EXIT_SUCCESS;
}
