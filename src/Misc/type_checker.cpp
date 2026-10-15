#pragma once

#include "Interfaces/ITypeVisitor.h"
#include <iostream>
#include "AST.h"

#include <typeinfo>
#include "utility"

class TypeChecker : public ITypeVisitor
{

public:
    TypeChecker() = default;

    compiler::DataType visit(compiler::BinaryExpression *binary_expr) const override
    {
        std::cout << "we doing type checking on an binary expr\n";

        auto *left = dynamic_cast<compiler::NumberExpression *>(binary_expr->left.get());
        auto *right = dynamic_cast<compiler::NumberExpression *>(binary_expr->right.get());

        visit(left);
        visit(right);
        
        if (dynamic_cast<compiler::IntType*>(left->Type.get()) &&
        dynamic_cast<compiler::IntType*>(right->Type.get()))
        {
            std::cout << "Both operands are integers\n";
            binary_expr->setType(std::make_unique<compiler::IntType>());
            // This node has integer type.
        }
        
        return *binary_expr->Type;

        // Check the types here, depending on your Type definition.
    };

    compiler::DataType visit(compiler::NumberExpression *number_expr) const override
    {
        std::cout << "we doing type checking on an number expr\n";
        number_expr->setType(std::make_unique<compiler::IntType>());

        return *number_expr->Type;
    };

    void do_type_checking(std::unique_ptr<compiler::Program> &program)
    {
        std::cout << "we doing type checking\n";
        for (auto &node : program->nodes)
        {
            std::cout << typeid(*node).name() << '\n';

            if (auto *binary_expr = dynamic_cast<compiler::BinaryExpression *>(node.get()))
            {
                visit(binary_expr);
            }
            else if (auto *number_expr = dynamic_cast<compiler::NumberExpression *>(node.get()))
            {
                visit(number_expr);
            }
        }
    }
};
