#include <iostream>
#include <fstream>
#include <memory>
#include <compiler.hpp>

int main(int numOfArg, char *args[]) {
    if (numOfArg != 5)
    {
        std::cout <<"Uruchamianie:\n"
                  << args[0] << " -i PLIKZKODEM -o WYJSCIE\n";
        return 0;
    }

    std::string codeFile = args[2];
    std::string outFile = args[4];

    std::ifstream code;
    code.open(codeFile);

    std::ofstream out;
    out.open(outFile);

    auto compiler = std::make_unique<jftt::compiler>();

    compiler->run(code, out);

    return 0;
}
