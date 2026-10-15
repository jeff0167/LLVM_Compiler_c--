#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "DataTypes.h"

namespace compiler
{

// ============================================================
// ASTNode
// ============================================================

class ASTNode
{
    public:
        virtual ~ASTNode() = default;

        virtual void print(int indent = 0) const = 0;

         std::unique_ptr<compiler::DataType> Type;

        void setType(std::unique_ptr<compiler::DataType> type)
        {
            Type = std::move(type);
        }

        //virtual int Visit() const = 0; // Visitor pattern

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
        //int Visit() const override; // Visitor pattern

        std::string value;
    private:
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

        std::string value;
    private:
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

        std::string name;
    private:
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

        std::unique_ptr<Expression> left;
        std::string operatorSymbol;
        std::unique_ptr<Expression> right;
    private:
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
        void addNode(std::unique_ptr<ASTNode> node)
        {
            nodes.push_back(std::move(node));
        }

        void print(int indent = 0) const override;

        std::vector<std::unique_ptr<ASTNode>> nodes;
    private:
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

        std::string type;
        std::string name;
        std::unique_ptr<Expression> initializer;
    private:
};

}
