#pragma once
#include "DataTypes.h"

class ITypeVisitor
{
    public:
    virtual ~ITypeVisitor() = default;

    virtual void visit(compiler::BinaryExpression* binary_expr) const = 0;
    virtual void visit(compiler::NumberExpression* number_expr) const = 0;
    virtual void visit(compiler::ExpressionStatement* expression_stmt) const = 0; 
    
};
