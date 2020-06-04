#include <iostream>
#include <fstream>
#include "utils/ast.hpp"
#include "codegen/CodeGenContext.h"
#include "parser.hpp"

extern FILE *yyin;
extern std::shared_ptr<ast::Program> astRoot;
std::ofstream astDot("../result/AST.dot");
std::ofstream codegenOutput("../result/codegen");

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "[Usage]: ./[elf] [source] [destination]" << std::endl;
        return 0;
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == 0)
    {
        std::cerr << "Cannot open file" << argv[1] << std::endl;
        exit(0);
    }
    yyin = fp;
    ast::parser p;
    p.parse();

    auto genContext = new CodeGenContext();
    astRoot->printSelf("main");
    genContext->generateCode(*(astRoot.get()));
    genContext->outputCode(argv[2]);

    fclose(fp);
    return 0;
}