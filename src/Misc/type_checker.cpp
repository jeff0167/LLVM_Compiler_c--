#pragma once

#include "Interfaces/ITypeVisitor.h"
#include <iostream>
#include "AST.h"

#include <typeinfo>

class TypeChecker : public ITypeVisitor
{

public:
    TypeChecker() = default;

    void visit(compiler::BinaryExpression *binary_expr) const override
    {
        std::cout << "we doing type checking on an binary expr";
        if (auto *left = dynamic_cast<compiler::NumberExpression *>(binary_expr->left.get()))
        {
            visit(left);
        }
        if (auto *right = dynamic_cast<compiler::NumberExpression *>(binary_expr->right.get()))
        {
            visit(right);
        }
    };

    void visit(compiler::NumberExpression *number_expr) const override
    {
        std::cout << "we doing type checking on an number expr";
    };

    void visit(compiler::ExpressionStatement *expression_stmt) const override
    {
        if (auto *binary_expr =
                dynamic_cast<compiler::BinaryExpression *>(expression_stmt->expression.get()))
        {
            visit(binary_expr);
        }
    };

    void do_type_checking(std::unique_ptr<compiler::Program> &program)
    {
        std::cout << "we doing type checking";
        for (auto &node : program->nodes)
        {
            std::cout << typeid(*node).name() << '\n';

            if (auto *binary_expr =
                    dynamic_cast<compiler::BinaryExpression *>(node.get()))
            {
                visit(binary_expr);
            }

            else if (auto *number_expr =
                         dynamic_cast<compiler::NumberExpression *>(node.get()))
            {
                visit(number_expr);
            }

            else if (auto *expression_stmt =
                         dynamic_cast<compiler::ExpressionStatement *>(node.get()))
            {
                visit(expression_stmt);
            }
        }
    }
};
