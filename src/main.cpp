#include "Emulator.h"

extern std::vector<std::shared_ptr<syntax_tree::ASTNode>> getTokens(char* arg);
extern syntax_tree::AST analize(std::vector<std::shared_ptr<syntax_tree::ASTNode>> t);

int main(int argc, char* argv[]) 
{   
    std::cout << "-----------------------------\n";
    std::cout << "-----------Tokens:-----------\n";
    std::cout << "-----------------------------\n";
    std::vector<std::shared_ptr<syntax_tree::ASTNode>> tokens = getTokens(argv[1]);
    for (auto t : tokens) { t->print(); }
    std::cout << "\n";

    std::cout << "-----------------------------\n";
    std::cout << "----Abstract syntax tree:----\n";
    std::cout << "-----------------------------\n";
    syntax_tree::AST ast = analize(tokens);
    ast.print();

    std::cout << "-----------------------------\n";
    std::cout << "------------Result:----------\n";
    std::cout << "-----------------------------\n";
    Emulator* e = new Emulator();
    syntax_tree::AST result = e->eval(ast);
    result.print();
}