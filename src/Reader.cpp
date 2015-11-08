#include "Reader.hpp"
#include <iostream>
#include <cassert>

ReaderString::ReaderString(const void* valueAttr)
:m_type((const Type*)valueAttr)
,m_size(0)
{}

Type ReaderString::getType() const
{
  return *m_type; //todo
}

void ReaderString::parser() const
{
  const unsigned char* current = (unsigned char*)m_type + sizeof(Type);
  
  if(*m_type == Type::STRING8)
  {
    m_size = *((uint8_t*)current);
    current += sizeof(uint8_t);
  }
  else if(*m_type == Type::STRING16)
  {
    m_size = *((uint16_t*)current);
    current += sizeof(uint16_t);
  }
  else if(*m_type == Type::STRING32)
  {
    m_size = *((uint32_t*)current);
    current += sizeof(uint32_t);
  }
  else if(*m_type == Type::STRING64)
  {
    m_size = *((uint64_t*)current);
    current += sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }
  m_str = (const char*)current;
}



ReaderVector::ReaderVector(const void* attr)
: m_type((const Type*)attr)
, m_size(0)
{
}

Type ReaderVector::getType() const
{
  return *m_type; //todo
}

void ReaderVector::parser() const
{
  const unsigned char* current = (unsigned char*)m_type + sizeof(Type);
  
  if(*m_type == Type::VECTOR8)
  {
    m_size = *((uint8_t*)current);
    current += sizeof(uint8_t);
  }
  else if(*m_type == Type::VECTOR16)
  {
    m_size = *((uint16_t*)current);
    current += sizeof(uint16_t);
  }
  else if(*m_type == Type::VECTOR32)
  {
    m_size = *((uint32_t*)current);
    current += sizeof(uint32_t);
  }
  else if(*m_type == Type::VECTOR64)
  {
    m_size = *((uint64_t*)current);
    current += sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }

  if(m_size == 0)
  {
    return;
  }
  
  const unsigned char* end =  current + m_size;

  while(current < end)
  {
    uint64_t size = 0;
    IReader* elemment = read(current,&size);
    m_elements.push_back(elemment);
    current += size;
  }
}

std::vector<IReader*>::const_iterator ReaderVector::begin() const
{
  if (m_elements.empty())
  {
    parser();
  }
  return m_elements.begin();
}

std::vector<IReader*>::const_iterator ReaderVector::end() const
{
  if (m_elements.empty())
  {
    parser();
  }
  return m_elements.end();
}



ReaderMap::ReaderMap(const void* attr)
:m_type((const Type*)attr)
,m_size(0)
{
}
Type ReaderMap::getType() const
{
  return *m_type; //todo
}

std::map<std::string,IReader*>::const_iterator ReaderMap::begin() const
{
  if (m_elements.empty())
  {
    parser();
  }
  return m_elements.begin();
}

std::map<std::string,IReader*>::const_iterator ReaderMap::end() const
{
  if (m_elements.empty())
  {
    parser();
  }
  return m_elements.end();
}

void ReaderMap::parser() const
{
  const unsigned char* current = (unsigned char*)m_type + sizeof(Type);
  
  if(*m_type == Type::MAP8)
  {
    m_size = *((uint8_t*)current);
    current += sizeof(uint8_t);
  }
  else if(*m_type == Type::MAP16)
  {
    m_size = *((uint16_t*)current);
    current += sizeof(uint16_t);
  }
  else if(*m_type == Type::MAP32)
  {
    m_size = *((uint32_t*)current);
    current += sizeof(uint32_t);
  }
  else if(*m_type == Type::MAP64)
  {
    m_size = *((uint64_t*)current);
    current += sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }
  
  if(m_size == 0)
  {
    return;
  }
  
  const unsigned char* end =  current + m_size;

  while(current < end)
  {
    // ReaderString
    uint16_t* lengthStringKey = (uint16_t*)current;
    current += sizeof(uint16_t);
    std::string key((const char*)current,*lengthStringKey);
    current += *lengthStringKey;

    // read value
    uint64_t size = 0;
    IReader* elemment = read(current,&size);

    m_elements.insert(std::make_pair(key,elemment));
    current += size;
  }
}


IReader* read(const void* data, uint64_t* size )
{
  Type* v = (Type*)data;
  switch(*v)
  {
    case Type::INT8:
    {
      auto value = new ReaderInt8(data);
      *size = sizeof(AttributeValue<Type::INT8, int8_t>);
      return value;
    }
    case Type::INT16:
    {
      auto value = new ReaderInt16(data);
      *size = sizeof(AttributeValue<Type::INT16, int16_t>);
      return value;
    }
    case Type::INT32:
    {
      auto value = new ReaderInt32(data);
      *size = sizeof(AttributeValue<Type::INT32, int32_t>);
      return value;
    }
    case Type::INT64:
    {
      auto value = new ReaderInt64(data);
      *size = sizeof(AttributeValue<Type::INT64, int64_t>);
      return value;
    }
    //--
    case Type::UINT8:
    {
      auto value = new ReaderUInt8(data);
      *size = sizeof(AttributeValue<Type::UINT8, uint8_t>);
      return value;
    }
    case Type::UINT16:
    {
      auto value = new ReaderUInt16(data);
      *size = sizeof(AttributeValue<Type::UINT16, uint16_t>);
      return value;
    }
    case Type::UINT32:
    {
      auto value = new ReaderUInt32(data);
      *size = sizeof(AttributeValue<Type::UINT32, uint32_t>);
      return value;
    }
    case Type::UINT64:
    {
      auto value = new ReaderUInt64(data);
      *size = sizeof(AttributeValue<Type::UINT64, uint64_t>);
      return value;
    }
    case Type::FLOAT:
    {
      auto value = new ReaderFloat(data);
      *size = sizeof(AttributeValue<Type::FLOAT, float>);
      return value;
    }
    case Type::DOUBLE:
    {
      auto value = new ReaderDouble(data);
      *size = sizeof(AttributeValue<Type::DOUBLE, double>);
      return value;
    }
    case Type::STRING8:
    {
      auto value = new ReaderString(data);
      *size = sizeof(Type) + sizeof(uint8_t) + value->length();
      return value;
    }
    case Type::STRING16:
    {
      auto value = new ReaderString(data);
      *size = sizeof(Type) + sizeof(uint16_t) + value->length();
      return value;
    }
    case Type::STRING32:
    {
      auto value = new ReaderString(data);
      *size = sizeof(Type) + sizeof(uint32_t) + value->length();
      return value;
    }
    case Type::STRING64:
    {
      auto value = new ReaderString(data);
      *size = sizeof(Type) + sizeof(uint64_t) + value->length();
      return value;
    }
    case Type::VECTOR8:
    {
      auto value = new ReaderVector(data);
      *size = sizeof(Type) + sizeof(uint8_t) + value->getBytesSize();
      return value;
    }
    case Type::VECTOR16:
    {
      auto value = new ReaderVector(data);
      *size = sizeof(Type) + sizeof(uint16_t) + value->getBytesSize();
      return value;
    }
    case Type::VECTOR32:
    {
      auto value = new ReaderVector(data);
      *size = sizeof(Type) + sizeof(uint32_t) + value->getBytesSize();
      return value;
    }
    case Type::VECTOR64:
    {
      auto value = new ReaderVector(data);
      *size = sizeof(Type) + sizeof(uint64_t) + value->getBytesSize();
      return value;
    }
    case Type::MAP8:
    {
      auto value = new ReaderMap(data);
      *size = sizeof(Type) + sizeof(uint8_t) + value->getBytesSize();
      return value;
    }
    case Type::MAP16:
    {
      auto value = new ReaderMap(data);
      *size = sizeof(Type) + sizeof(uint16_t) + value->getBytesSize();
      return value;
    }
    case Type::MAP32:
    {
      auto value = new ReaderMap(data);
      *size = sizeof(Type) + sizeof(uint32_t) + value->getBytesSize();
      return value;
    }
    case Type::MAP64:
    {
      auto value = new ReaderMap(data);
      *size = sizeof(Type) + sizeof(uint64_t) + value->getBytesSize();
      return value;
    }
    case Type::BOOL:
    {
      auto value = new ReaderBool(data);
      *size = sizeof(AttributeValue<Type::BOOL, bool>);
      return value;
    }
    case Type::NULL_:
    {
      IReader* value = nullptr;
      *size = sizeof(Type);
      return value;
    }
    default:
    return nullptr;
  };
  return nullptr;
}
