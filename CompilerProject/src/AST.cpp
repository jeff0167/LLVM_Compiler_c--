#include "AST.h"

#include <iostream>

namespace Compiler {

// ============================================================
// Program
// ============================================================

void Program::print(int indent) const
{
    printIndent(indent);
    std::cout << "Program\n";

    for (const auto& statement : statements) {
        statement->print(indent + 1);
    }
}


// ============================================================
// Expression statement
// ============================================================

void ExpressionStatement::print(int indent) const
{
    printIndent(indent);
    std::cout << "ExpressionStatement\n";

    expression->print(indent + 1);
}


// ============================================================
// Variable declaration
// ============================================================

void VariableDeclaration::print(int indent) const
{
    printIndent(indent);
    std::cout << "VariableDeclaration\n";

    printIndent(indent + 1);
    std::cout << "Type: " << type << "\n";

    printIndent(indent + 1);
    std::cout << "Name: " << name << "\n";

    if (initializer) {
        printIndent(indent + 1);
        std::cout << "Initializer:\n";
        initializer->print(indent + 2);
    }
}

// ============================================================
// Number expression
// ============================================================

void NumberExpression::print(int indent) const
{
    printIndent(indent);
    std::cout << "NumberExpression: " << value << "\n";
}


// ============================================================
// String expression
// ============================================================

void StringExpression::print(int indent) const
{
    printIndent(indent);
    std::cout << "StringExpression: " << value << "\n";
}


// ============================================================
// Identifier expression
// ============================================================

void IdentifierExpression::print(int indent) const
{
    printIndent(indent);
    std::cout << "IdentifierExpression: " << name << "\n";
}


// ============================================================
// Binary expression
// ============================================================

void BinaryExpression::print(int indent) const
{
    printIndent(indent);
    std::cout << "BinaryExpression: " << operatorSymbol << "\n";

    printIndent(indent + 1);
    std::cout << "Left:\n";
    left->print(indent + 2);

    printIndent(indent + 1);
    std::cout << "Right:\n";
    right->print(indent + 2);
}


} // namespace Compiler
