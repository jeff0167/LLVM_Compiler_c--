#pragma once

namespace compiler
{

    class DataType
    {
    public:
        virtual ~DataType() = default;
    };

    class IntType : public DataType
    {
    public:
        IntType() = default;
     
        ~IntType() = default;
    };

    class BoolType : public DataType
    {
    public:
        BoolType() = default;

        ~BoolType() = default;
    };
}