%{
#include "AST.h"

void set_input_file(yyscan_t scanner, const char* filename);

static int current_line = 1;
static int current_column = 1;

struct token
{
    enum token_kind_type
    {
        YYEMPTY = -2,
        YYEOF = 0,                     // "end of file"
        YYerror = 256,                 // error
        YYUNDEF = 257,                 // "invalid token"
        T_LITERAL_FLOAT = 258,         // T_LITERAL_FLOAT
        T_PLUS = 259,                  // T_PLUS
        T_MINUS = 260,                 // T_MINUS
        T_MUL = 261,                   // T_MUL
        T_DIV = 262,                   // T_DIV
        UMINUS = 263,                  // UMINUS
        T_PARENTHESIS_OPEN = 264,      // T_PARENTHESIS_OPEN
        T_PARENTHESIS_CLOSE = 265,     // T_PARENTHESIS_CLOSE
        T_END_OF_FILE = 266            // T_END_OF_FILE
    };
    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type yytokentype;
};

%}

%option reentrant batch noyywrap yylineno nodefault outfile="Scanner.cpp" header="Scanner.hpp"

%%

;.*                    { /* ignore comment */ }

"("                    { return token::T_PARENTHESIS_OPEN; }
")"                    { return token::T_PARENTHESIS_CLOSE; }

"+"                    { return token::T_PLUS; }
"-"                    { return token::T_MINUS; }
"*"                    { return token::T_MUL; }
"/"                    { return token::T_DIV; }

[0-9]+("."[0-9]+)? { return token::T_LITERAL_FLOAT; } 

\n                     { current_line++; current_column = 1; }
\r                     { current_column = 1; }
[ \t]+                 {}

<<EOF>>                { return token::T_END_OF_FILE; }
.                      { return yytext[0]; }

%%

void set_input_file(yyscan_t scanner, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        yyset_in(file, scanner);
    } else {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }
}

std::vector<std::shared_ptr<syntax_tree::ASTNode>> getTokens(char* arg) {
    yyscan_t scanner;
    yylex_init(&scanner);
    
    set_input_file(scanner, arg);
    
    std::vector<std::shared_ptr<syntax_tree::ASTNode>> result;
    int token;
    while ((token = yylex(scanner)) != token::T_END_OF_FILE) {
        if (token == token::T_LITERAL_FLOAT) {
            result.push_back(
                std::make_shared<syntax_tree::LiteralFloat>("NUM", token, atof(yyget_text(scanner)))
            );
        }
        else {
            result.push_back(
                std::make_shared<syntax_tree::ASTNode>(std::string(yyget_text(scanner), yyget_leng(scanner)), token)
            );
        }
    }

    result.push_back(
        std::make_shared<syntax_tree::ASTNode>("EOF", token::T_END_OF_FILE)
    );

    yylex_destroy(scanner);

    return result;
}