#include "Emulator.h"

Node Emulator::eval(Node e) {
    std::string t = e->getNodeType();
    if      (t == "NUM") { return e; }
    else if (t == "+")   { return evalAddNode(e); }
    else if (t == "-")   { return evalSubNode(e); }
    else if (t == "*")   { return evalMulNode(e); }
    else if (t == "/")   { return evalDivNode(e); }

    throw std::runtime_error("Unknown node type");
}

LiteralFloat Emulator::evalAddNode(Node add) {
    auto left = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(add->getStatement(0)));
    auto right = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(add->getStatement(1)));
    return std::make_shared<syntax_tree::LiteralFloat>("NUM", left->getValue() + right->getValue());
}

LiteralFloat Emulator::evalSubNode(Node sub) {
    if (sub->getStatementCount() == 1) {
        auto first = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(sub->getStatement(0)));
        return std::make_shared<syntax_tree::LiteralFloat>("NUM", -first->getValue());
    }
    auto left = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(sub->getStatement(0)));
    auto right = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(sub->getStatement(1)));
    return std::make_shared<syntax_tree::LiteralFloat>("NUM", left->getValue() - right->getValue());
}

LiteralFloat Emulator::evalMulNode(Node mul) {
    auto left = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(mul->getStatement(0)));
    auto right = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(mul->getStatement(1)));
    return std::make_shared<syntax_tree::LiteralFloat>("NUM", left->getValue() * right->getValue());
}

LiteralFloat Emulator::evalDivNode(Node div) {
    auto left = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(div->getStatement(0)));
    auto right = std::dynamic_pointer_cast<syntax_tree::LiteralFloat>(eval(div->getStatement(1)));
    return std::make_shared<syntax_tree::LiteralFloat>("NUM", left->getValue() / right->getValue());
}
    
syntax_tree::AST Emulator::eval(syntax_tree::AST ast) {
    Node root = eval(ast.getRoot());
    return syntax_tree::AST(root);
}