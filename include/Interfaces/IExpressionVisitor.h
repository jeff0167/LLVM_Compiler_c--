#include "AST.h"


    class IExpressionVisitor
    {
         public:
             virtual void Visit(compiler::BinaryExpression* expr) = 0;
    };
