%require "3.7.4"
%language "C++"
%defines "Parser.hpp"
%output "Parser.cpp"

%define api.parser.class {Parser}
%define api.namespace {lisp_for_kids}
%define api.value.type variant
%parse-param {syntax_tree::AST& result}


%code requires {
    #include "AST.h"
}

%code provides
{
    #define YY_DECL \
        int yylex(lisp_for_kids::Parser::semantic_type *yylval)
    YY_DECL;
}

%token <std::shared_ptr<syntax_tree::ASTNode>> T_LITERAL_FLOAT
%left  <std::shared_ptr<syntax_tree::ASTNode>> T_PLUS T_MINUS // low priority
%left  <std::shared_ptr<syntax_tree::ASTNode>> T_MUL T_DIV // medium priority
%right UMINUS // high priority
%token <std::shared_ptr<syntax_tree::ASTNode>> T_PARENTHESIS_OPEN T_PARENTHESIS_CLOSE
%token <std::shared_ptr<syntax_tree::ASTNode>> T_END_OF_FILE

%type <std::shared_ptr<syntax_tree::ASTNode>> s expr num

%%

s: expr T_END_OF_FILE {
    result = syntax_tree::AST($1);
    YYACCEPT;
};

expr: num { $$ = $1; }
    | expr T_PLUS expr { 
        $2->addStatement($1);
        $2->addStatement($3);
        $$ = $2; 
    }
    | expr T_MINUS expr { 
        $2->addStatement($1);
        $2->addStatement($3);
        $$ = $2; 
    }
    | expr T_MUL expr {
        $2->addStatement($1);
        $2->addStatement($3);
        $$ = $2;  
    }
    | expr T_DIV expr { 
        $2->addStatement($1);
        $2->addStatement($3);
        $$ = $2;  
    }
    | T_MINUS expr %prec UMINUS { 
        $1->addStatement($2);
        $$ = $1;  
    }
    | T_PARENTHESIS_OPEN expr T_PARENTHESIS_CLOSE { $$ = $2; };

num: T_LITERAL_FLOAT { $$ = $1; };

%%

std::vector<std::shared_ptr<syntax_tree::ASTNode>> tokens;
int pos = 0;

syntax_tree::AST analize(std::vector<std::shared_ptr<syntax_tree::ASTNode>> t) {
    syntax_tree::AST result;
    tokens = t;
    lisp_for_kids::Parser parser { result };
    if (parser.parse() == 0) {
        std::cout << "Parse success.\n";
    }
    else {
        std::cerr << "Parse error!\n";
    }
    return result;
}

int yylex(lisp_for_kids::Parser::semantic_type* yylval) {
    if (pos >= tokens.size()) return 0;
    
    std::shared_ptr<syntax_tree::ASTNode> t = tokens[pos++];

    yylval->emplace<std::shared_ptr<syntax_tree::ASTNode>>(t);

    return t->getToken();
}

void lisp_for_kids::Parser::error(const std::string& msg) {
    std::cerr << msg << "\n";
}