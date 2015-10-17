#ifndef _BINJSON_SERIALIZE_H_
#define _BINJSON_SERIALIZE_H_

#include <cstdint>
#include "Attribute.hpp"

class ISerialize
{
  public:
    virtual ~ISerialize(){}
    virtual void* serialize(void* data) const = 0;
    virtual uint64_t getBytesSize() const = 0;
    virtual Type getType() const = 0;
};
void* writeBuffer(void* buffer, void *data, size_t len);

template<Type type, typename Value>
class SerializeValue : public ISerialize
{
protected:
  AttributeValue<type,Value> m_value;
public:
  SerializeValue(Value&& value)
  : m_value(std::move(AttributeValue<type,Value>(std::move(value))))
  {}

  ~SerializeValue(){};

  void* serialize(void* data) const override
  {
    return writeBuffer(data,(void*)&m_value,sizeof(AttributeValue<type,Value>));
  }

  Type getType() const override
  {
    return m_value.getType();
  }

  uint64_t getBytesSize() const override
  {
    return sizeof(AttributeValue<type,Value>);
  }
};


class SerializeString : public ISerialize
{
protected:
  std::string m_data;
  Type m_type;
public:
  SerializeString( const std::string& data );

  ~SerializeString();

  void* serialize(void* data) const override;

  Type getType() const override;

  uint64_t getBytesSize() const override;
};



class SerializeVector : public ISerialize
{
protected:
  std::vector<ISerialize*> m_value;
  Type m_type;
  uint64_t m_size;
public:
  SerializeVector(std::vector<ISerialize*>&& value);

  ~SerializeVector();

  uint64_t getBytesSize() const override;

  void* serialize(void* data) const  override;

  Type getType() const override;
};


class SerializeMap : public ISerialize
{
protected:
  std::map<std::string,ISerialize*> m_value;
  Type m_type;
  uint64_t m_size;
public:
  SerializeMap(  std::map<std::string,ISerialize*>&& value);

  ~SerializeMap();

  uint64_t getBytesSize() const override;

  void* serialize(void* data) const override;

  Type getType() const override;
};


using SerializeInt8 = SerializeValue<Type::INT8, int8_t>;
using SerializeInt16 = SerializeValue<Type::INT16, int16_t>;
using SerializeInt32 = SerializeValue<Type::INT32, int32_t>;
using SerializeInt64 = SerializeValue<Type::INT64, int64_t>;


using SerializeUInt8 = SerializeValue<Type::UINT8, uint8_t>;
using SerializeUInt16 = SerializeValue<Type::UINT16, uint16_t>;
using SerializeUInt32 = SerializeValue<Type::UINT32, uint32_t>;
using SerializeUInt64 = SerializeValue<Type::UINT64, uint64_t>;

using SerializeFloat = SerializeValue<Type::FLOAT, float>;
using SerializeDouble = SerializeValue<Type::DOUBLE, double>;
using SerializeBool = SerializeValue<Type::BOOL, bool>;

#endif
