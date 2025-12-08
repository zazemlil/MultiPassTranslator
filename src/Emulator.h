#pragma once

#include "AST.h"

typedef std::shared_ptr<syntax_tree::ASTNode> Node;
typedef std::shared_ptr<syntax_tree::LiteralFloat> LiteralFloat;

class Emulator {
private:
    Node eval(Node e);

    LiteralFloat evalAddNode(Node add);
    LiteralFloat evalSubNode(Node sub);
    LiteralFloat evalMulNode(Node mul);
    LiteralFloat evalDivNode(Node dive);
    
public:
    syntax_tree::AST eval(syntax_tree::AST ast);
};