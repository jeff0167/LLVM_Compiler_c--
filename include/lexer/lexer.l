%{
#include "parser.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
%}

%option noyywrap
%option nounput
%option noinput

%%

[ \t\r]+ {
    /* Ignore spaces, tabs and carriage returns */
}

\n {
    std::cout << "LEXER: END\n";
    return END;
}


[0-9]+ {
    std::cout << "LEXER: NUMBER = " << yytext << "\n";
    yylval.number = std::stoi(yytext);
    return NUMBER;
}


[a-zA-Z_][a-zA-Z0-9_]* {
    if (std::strcmp(yytext, "int") == 0)
        return INT;

    if (std::strcmp(yytext, "float") == 0)
        return FLOAT;

    if (std::strcmp(yytext, "void") == 0)
        return VOID;

    if (std::strcmp(yytext, "char") == 0)
        return CHAR;

    if (std::strcmp(yytext, "bool") == 0)
        return BOOL;

    yylval.string = ::strdup(yytext);
    return IDENTIFIER;
}


"+=" { return PLUS_ASSIGN; }
"-=" { return MINUS_ASSIGN; }
"*=" { return MUL_ASSIGN; }
"/=" { return DIV_ASSIGN; }

"==" { return EQUALS_EQUALS; }
"<=" { return LESS_EQUALS; }
">=" { return GREATER_EQUALS; }
"->" { return ARROW; }

"+" {
    std::cout << "LEXER: PLUS\n";
    return PLUS;
}

"-" { return MINUS; }
"*" { return STAR; }
"/" { return SLASH; }

"=" { return ASSIGN; }

"<" { return LESS; }
">" { return GREATER; }

";" { return SEMICOLON; }
"," { return COMMA; }
":" { return COLON; }

"(" { return LPAREN; }
")" { return RPAREN; }

"{" { return LBRACE; }
"}" { return RBRACE; }

"[" { return LBRACKET; }
"]" { return RBRACKET; }

"." { return DOT; }


<<EOF>> {
    std::cout << "LEXER: EOF\n";
    return 0;
}


. {
    std::cerr
        << "Unknown character: '"
        << yytext
        << "'\n";
}

%%
