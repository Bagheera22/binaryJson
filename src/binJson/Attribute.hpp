#pragma once
#include <binJson/IAttribute.hpp>
#include <functional>

namespace binJson
{
template <InternalType t, typename Value>
struct AttributeValue : public IAttribute
{
private:
    Value m_value;

public:
    AttributeValue(Value&& value)
        : IAttribute(t)
        , m_value(std::move(value)){};

    ~AttributeValue(){};

    inline const Value& getValue() const
    {
        return m_value;
    }
};
}