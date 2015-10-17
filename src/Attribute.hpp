#ifndef _BINJSON_ATTRIBUTE_H_
#define _BINJSON_ATTRIBUTE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <map>

enum class Type : uint8_t //todo
{
  INT8,INT16,INT32,INT64,
  UINT8,UINT16,UINT32,UINT64,
  FLOAT, DOUBLE, BOOL,
  VECTOR8, VECTOR16, VECTOR32, VECTOR64,
  MAP8, MAP16, MAP32, MAP64,
  STRING8, STRING16, STRING32, STRING64
};

class IAttribute
{
private:
  Type m_type;
public:
  IAttribute(Type type)
  :m_type(type){};

  ~IAttribute(){};

  inline Type getType() const
  {
    return m_type;
  }
};

template<Type t, typename Value>
struct AttributeValue : public IAttribute
{
private:
  Value m_value;
public:
  AttributeValue(Value&& value)
  : IAttribute(t)
  , m_value(std::move(value))
  {};

  ~AttributeValue(){};

  inline Value getValue() const
  {
    return m_value;
  }

  inline Type getType() const
  {
    return t;
  }
};

/*
template<Type type, typename Value>
class ComplexAttribute : public IAttribute
{
private:
  Value m_bytesSize;
public:
  ComplexAttribute()
  : IAttribute(type)
  {}

  ~ComplexAttribute(){};

  inline Value getSize() const
  {
    return m_bytesSize;
  }
  
  inline void setSize(Value size)
  {
    m_bytesSize = size;
  }
};*/


#endif
