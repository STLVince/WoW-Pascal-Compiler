#include <iostream>
#include <fstream>
#include <cstdlib>
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
        std::cout << "[Usage]: ./[elf] [source] [destination] [-o]" << std::endl;
        return 0;
    }

    std::cout << "Parsing start..." << std::endl;
    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == 0)
    {
        std::cerr << "Cannot open file" << argv[1] << std::endl;
        exit(0);
    }
    yyin = fp;
    ast::parser p;
    p.parse();

    bool optimize;
    if (argc > 3 && strcmp(argv[3], "-o") == 0)
    {
        optimize = true;
    }
    auto genContext = new CodeGenContext(optimize);
    astRoot->printSelf("main");
    system("dot -Tpng ../result/AST.dot -o ../result/AST.png");
    std::cout << "Parsing success! AST tree is saved in ../result/AST.png" << std::endl;

    std::cout << "Code generation start..." << std::endl;    
    genContext->generateCode(*(astRoot.get()));
    genContext->outputCode(argv[2]);
    std::cout << "Code generation success! Result is saved in " << argv[2] << std::endl;

    fclose(fp);
    return 0;
}