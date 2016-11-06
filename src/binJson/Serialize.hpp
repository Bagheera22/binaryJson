#ifndef _BINJSON_SERIALIZE_H_
#define _BINJSON_SERIALIZE_H_

#include <cstdint>
#include "Attribute.hpp"
#include <string>
#include <vector>
#include <map>

namespace binJson
{
class ISerialize
{
public:
    virtual ~ISerialize() {}
    virtual void* serialize(void* data) const = 0;
    virtual uint64_t getBytesSize() const = 0;
    virtual InternalType getInternalType() const = 0;
};

void* writeBuffer(void* buffer, void* data, size_t len);

template <InternalType type, typename Value>
class SerializeValue : public ISerialize
{
protected:
    AttributeValue<type, Value> m_value;

public:
    SerializeValue(Value&& value)
        : m_value(std::move(AttributeValue<type, Value>(std::move(value))))
    {
    }

    ~SerializeValue(){};

    void* serialize(void* data) const override
    {
        return writeBuffer(data, (void*)&m_value, sizeof(AttributeValue<type, Value>));
    }

    InternalType getInternalType() const override
    {
        return m_value.getType();
    }

    uint64_t getBytesSize() const override
    {
        return sizeof(AttributeValue<type, Value>);
    }
};

class SerializeString : public ISerialize
{
protected:
    std::string m_data;
    InternalType m_type;

public:
    SerializeString(const std::string& data);

    ~SerializeString();

    void* serialize(void* data) const override;

    InternalType getInternalType() const override;

    uint64_t getBytesSize() const override;
};

class SerializeVector : public ISerialize
{
protected:
    std::vector<ISerialize*> m_value;
    InternalType m_type;
    uint64_t m_size;

public:
    SerializeVector(const std::vector<ISerialize*>& value);

    ~SerializeVector();

    uint64_t getBytesSize() const override;

    void* serialize(void* data) const override;

    InternalType getInternalType() const override;
};

class SerializeMap : public ISerialize
{
protected:
    std::map<std::string, ISerialize*> m_value;
    InternalType m_type;
    uint64_t m_size;

public:
    SerializeMap(const std::map<std::string, ISerialize*>& value);

    ~SerializeMap();

    uint64_t getBytesSize() const override;

    void* serialize(void* data) const override;

    InternalType getInternalType() const override;
};

class SerializeNull : public ISerialize
{
protected:
    InternalType m_type;

public:
    SerializeNull();

    ~SerializeNull();

    void* serialize(void* data) const override;

    InternalType getInternalType() const override;

    uint64_t getBytesSize() const override;
};

using SerializeInt8 = SerializeValue<InternalType::INT8, int8_t>;
using SerializeInt16 = SerializeValue<InternalType::INT16, int16_t>;
using SerializeInt32 = SerializeValue<InternalType::INT32, int32_t>;
using SerializeInt64 = SerializeValue<InternalType::INT64, int64_t>;

using SerializeUInt8 = SerializeValue<InternalType::UINT8, uint8_t>;
using SerializeUInt16 = SerializeValue<InternalType::UINT16, uint16_t>;
using SerializeUInt32 = SerializeValue<InternalType::UINT32, uint32_t>;
using SerializeUInt64 = SerializeValue<InternalType::UINT64, uint64_t>;

using SerializeFloat = SerializeValue<InternalType::FLOAT, float>;
using SerializeDouble = SerializeValue<InternalType::DOUBLE, double>;
using SerializeBool = SerializeValue<InternalType::BOOL, bool>;
}

#endif
