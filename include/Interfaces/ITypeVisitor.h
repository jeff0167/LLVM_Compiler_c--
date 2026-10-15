#pragma once
#include "DataTypes.h"

class ITypeVisitor
{
    public:
    virtual ~ITypeVisitor() = default;

    virtual compiler::DataType visit(compiler::BinaryExpression* binary_expr) const = 0;
    virtual compiler::DataType visit(compiler::NumberExpression* number_expr) const = 0;
    
};
