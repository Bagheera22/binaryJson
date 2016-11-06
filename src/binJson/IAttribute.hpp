#pragma once

#include <cstdint>
namespace binJson
{
enum class InternalType : uint8_t
{
    NULL_ = 0,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT,
    DOUBLE,
    BOOL,
    VECTOR8,
    VECTOR16,
    VECTOR32,
    VECTOR64,
    MAP8,
    MAP16,
    MAP32,
    MAP64,
    STRING8,
    STRING16,
    STRING32,
    STRING64
};

enum class Type : uint8_t
{
    NULL_ = 0,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT,
    DOUBLE,
    BOOL,
    VECTOR,
    MAP,
    STRING
};

class IAttribute
{
private:
    InternalType m_type;

public:
    IAttribute(InternalType type)
        : m_type(type){};

    ~IAttribute(){};

    inline InternalType getType() const
    {
        return m_type;
    }
};
}