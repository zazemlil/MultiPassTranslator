#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace syntax_tree {

class ASTNode : public std::enable_shared_from_this<ASTNode> {
    std::string node_type;
    int token;
    std::vector<std::shared_ptr<ASTNode>> statements;

public:
    ASTNode() {}
    ASTNode(std::string t) : node_type(t) {}
    ASTNode(std::string t, int tok) : node_type(t), token(tok) {}
    
    virtual ~ASTNode() = default;

    std::string getNodeType() const { return node_type; }
    int getToken() const { return token; }
    size_t getStatementCount() const { return statements.size(); }

    std::shared_ptr<ASTNode>& getStatement(size_t index) { return statements.at(index); }
    void addStatement(std::shared_ptr<ASTNode> stmt) { statements.push_back(stmt); }

    void setStatements(std::vector<std::shared_ptr<ASTNode>>& new_statements) { this->statements = new_statements; }
    std::vector<std::shared_ptr<ASTNode>>& getStatements() { return statements; }
    
    void addStatements(std::vector<std::shared_ptr<ASTNode>>& new_statements) {
        this->statements.insert(
            std::end(this->statements),
            std::begin(new_statements),
            std::end(new_statements)
        );
    }

    virtual void printValue(std::ostream& os = std::cout) const { os << node_type; }

    void print(int indent = 0, std::ostream& os = std::cout) const {
        std::string indentStr = ""; 
        for (int i = 0; i < indent-1; i++) {indentStr += "    ";} 
        
        os << indentStr << "";
        this->printValue(os);
        os << '\n';
        
        for (const auto& stmt : statements) {
            stmt->print(indent + 2, os);
        }
    }

};


class AST {
private:
    std::shared_ptr<ASTNode> root = nullptr;

public:
    AST() {}
    AST(std::shared_ptr<ASTNode> rootNode) : root(rootNode) {}

    std::shared_ptr<ASTNode> getRoot() { return root; }
    bool isEmpty() const { return root == nullptr; }

    void print(std::ostream& os = std::cout) const {
        if (root) {
            root->print(0);
            std::cout << "\n";
        } else {
            std::cout << "AST is empty.\n\n";
        }
    }
};


class LiteralFloat : public ASTNode {
    float value;
public:
    void printValue(std::ostream& os = std::cout) const override { os << value; }
    float getValue() { return value; }
    LiteralFloat(std::string t, int v) : ASTNode(t), value(v) {}
    LiteralFloat(std::string t, int tok, float v) : ASTNode(t, tok), value(v) {}
};

};