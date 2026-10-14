#pragma once

namespace compiler
{

    class DataType
    {
    public:
        virtual ~DataType() = default;
    };

    class IntType : DataType
    {
    public:
        virtual ~IntType() = default;
    };

    class BoolType : DataType
    {
    public:
        virtual ~BoolType() = default;
    };
}