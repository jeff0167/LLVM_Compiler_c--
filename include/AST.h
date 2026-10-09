#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>


namespace Compiler
{

// ============================================================
// ASTNode
// ============================================================

class ASTNode
{
    public:
        virtual ~ASTNode() = default;

        virtual void print(int indent = 0) const = 0;

    protected:
        static void printIndent(int indent)
        {
            for (int i = 0; i < indent; ++i)
                std::cout << "  ";
        }
};


// ============================================================
// Expression
// ============================================================

class Expression : public ASTNode
{
};

// ============================================================
// Number expression
// ============================================================

class NumberExpression : public Expression
{
    public:
        explicit NumberExpression(const std::string& value)
            : value(value)
        {
        }

        void print(int indent = 0) const override;

    private:
        std::string value;
};


// ============================================================
// String expression
// ============================================================

class StringExpression : public Expression
{
    public:
        explicit StringExpression(const std::string& value)
            : value(value)
        {
        }

        void print(int indent = 0) const override;

    private:
        std::string value;
};


// ============================================================
// Identifier expression
// ============================================================

class IdentifierExpression : public Expression
{
    public:
        explicit IdentifierExpression(const std::string& name)
            : name(name)
        {
        }

        void print(int indent = 0) const override;

    private:
        std::string name;
};


// ============================================================
// Binary expression
// ============================================================

class BinaryExpression : public Expression
{
    public:
        BinaryExpression(
            std::unique_ptr<Expression> left,
            const std::string& operatorSymbol,
            std::unique_ptr<Expression> right
        )
            : left(std::move(left)),
            operatorSymbol(operatorSymbol),
            right(std::move(right))
        {
        }

        void print(int indent = 0) const override;

    private:
        std::unique_ptr<Expression> left;
        std::string operatorSymbol;
        std::unique_ptr<Expression> right;
};


// ============================================================
// Statement
// ============================================================

class Statement : public ASTNode
{
};


// ============================================================
// Program
// ============================================================

class Program : public ASTNode
{
    public:
        void addStatement(std::unique_ptr<Statement> statement)
        {
            statements.push_back(std::move(statement));
        }

        void print(int indent = 0) const override;

    private:
        std::vector<std::unique_ptr<Statement>> statements;
};


// ============================================================
// Expression statement
// ============================================================

class ExpressionStatement : public Statement
{
    public:
        explicit ExpressionStatement(
            std::unique_ptr<Expression> expression
        )
            : expression(std::move(expression))
        {
        }

        void print(int indent = 0) const override;

    private:
        std::unique_ptr<Expression> expression;
};


// ============================================================
// Variable declaration
// ============================================================

class VariableDeclaration : public Statement
{
    public:
        VariableDeclaration(
            const std::string& type,
            const std::string& name,
            std::unique_ptr<Expression> initializer
        )
            : type(type),
            name(name),
            initializer(std::move(initializer))
        {
        }

        void print(int indent = 0) const override;

    private:
        std::string type;
        std::string name;
        std::unique_ptr<Expression> initializer;
};

}
