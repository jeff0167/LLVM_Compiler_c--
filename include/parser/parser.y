%{
#include "AST.h"

#include <iostream>
#include <memory>

int yylex();

void yyerror(const char* message);
/*extern int yydebug;*/   /* deep parser debuging 1*/


std::unique_ptr<Compiler::Program> parsedProgram;
%}


%code requires
{
    #include "AST.h"
    #include <memory>
}
/*%define parse.trace*/   /* deep parser debuging 2 they go together*/


/* ============================================================
   Semantic values
   ============================================================ */

%union
{
    int number;
    char* string;

    Compiler::Expression* expression;
    Compiler::Statement* statement;
}


/* ============================================================
   Tokens
   ============================================================ */

%token <number> NUMBER
%token <string> IDENTIFIER
%token <string> STRING

%token INT
%token FLOAT
%token VOID
%token CHAR
%token BOOL

%token PLUS
%token MINUS
%token STAR
%token SLASH

%token PLUS_ASSIGN
%token MINUS_ASSIGN
%token MUL_ASSIGN
%token DIV_ASSIGN

%token ASSIGN
%token EQUALS_EQUALS

%token LESS
%token LESS_EQUALS
%token GREATER
%token GREATER_EQUALS

%token ARROW

%token SEMICOLON
%token COMMA
%token COLON

%token LPAREN
%token RPAREN

%token LBRACE
%token RBRACE

%token LBRACKET
%token RBRACKET

%token DOT

%token END


/* ============================================================
   Non-terminal types
   ============================================================ */

%type <expression> expression
%type <expression> primary_expression

%type <statement> statement
%type <statement> variable_declaration


/* ============================================================
   Precedence
   ============================================================ */

%left EQUALS_EQUALS

%left LESS LESS_EQUALS GREATER GREATER_EQUALS

%left PLUS MINUS

%left STAR SLASH

%right ASSIGN


/* ============================================================
   Start rule
   ============================================================ */

%start input


%%


/* ============================================================
   REPL input
   ============================================================ */

input
    :
      statement END
    {
        parsedProgram = std::make_unique<Compiler::Program>();

        parsedProgram->addStatement(
            std::unique_ptr<Compiler::Statement>($1)
        );
    }

    |

      statement SEMICOLON END
    {
        parsedProgram = std::make_unique<Compiler::Program>();

        parsedProgram->addStatement(
            std::unique_ptr<Compiler::Statement>($1)
        );
    }
    ;


/* ============================================================
   Statements
   ============================================================ */

statement
    :
      expression
    {
        $$ = new Compiler::ExpressionStatement(
            std::unique_ptr<Compiler::Expression>($1)
        );
    }

    |

      variable_declaration
    {
        $$ = $1;
    }
    ;


variable_declaration
    :
      INT IDENTIFIER ASSIGN expression
    {
        $$ = new Compiler::VariableDeclaration(
            "int",
            $2,
            std::unique_ptr<Compiler::Expression>($4)
        );

        std::free($2);
    }

    |

      FLOAT IDENTIFIER ASSIGN expression
    {
        $$ = new Compiler::VariableDeclaration(
            "float",
            $2,
            std::unique_ptr<Compiler::Expression>($4)
        );

        std::free($2);
    }

    |

      CHAR IDENTIFIER ASSIGN expression
    {
        $$ = new Compiler::VariableDeclaration(
            "char",
            $2,
            std::unique_ptr<Compiler::Expression>($4)
        );

        std::free($2);
    }

    |

      BOOL IDENTIFIER ASSIGN expression
    {
        $$ = new Compiler::VariableDeclaration(
            "bool",
            $2,
            std::unique_ptr<Compiler::Expression>($4)
        );

        std::free($2);
    }
    ;


/* ============================================================
   Expressions
   ============================================================ */

expression
    :
      primary_expression
    {
        $$ = $1;
    }

    |

      expression PLUS expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "+",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression MINUS expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "-",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression STAR expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "*",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression SLASH expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "/",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression EQUALS_EQUALS expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "==",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression LESS expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "<",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression LESS_EQUALS expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            "<=",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression GREATER expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            ">",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      expression GREATER_EQUALS expression
    {
        $$ = new Compiler::BinaryExpression(
            std::unique_ptr<Compiler::Expression>($1),
            ">=",
            std::unique_ptr<Compiler::Expression>($3)
        );
    }

    |

      LPAREN expression RPAREN
    {
        $$ = $2;
    }
    ;


/* ============================================================
   Primary expressions
   ============================================================ */

primary_expression
    :
      NUMBER
    {
        $$ = new Compiler::NumberExpression(
            std::to_string($1)
        );
    }

    |

      STRING
    {
        $$ = new Compiler::StringExpression($1);

        std::free($1);
    }

    |

      IDENTIFIER
    {
        $$ = new Compiler::IdentifierExpression($1);

        std::free($1);
    }
    ;


%%


/* ============================================================
   Error handling
   ============================================================ */

void yyerror(const char* message)
{
    std::cerr
        << "Parser error: "
        << message
        << '\n';
}
