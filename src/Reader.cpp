#include "Reader.hpp"
#include <iostream>
#include <cassert>

ReaderString::ReaderString(const void* valueAttr)
:m_type(InternalType::NULL_)
,m_size(0)
{
  assert(valueAttr != nullptr);
  m_type = *((InternalType*)valueAttr);
  const unsigned char* sizePointer = (const unsigned char*)valueAttr + sizeof(InternalType);
  
  if(m_type == InternalType::STRING8)
  {
    m_size = *((uint8_t*)sizePointer);
    m_str = (const char*)(sizePointer + sizeof(uint8_t));
  }
  else if(m_type == InternalType::STRING16)
  {
    m_size = *((uint16_t*)sizePointer);
    m_str = (const char*)(sizePointer + sizeof(uint16_t));
  }
  else if(m_type == InternalType::STRING32)
  {
    m_size = *((uint32_t*)sizePointer);
    m_str = (const char*)(sizePointer + sizeof(uint32_t));
  }
  else if(m_type == InternalType::STRING64)
  {
    m_size = *((uint64_t*)sizePointer);
    m_str = (const char*)(sizePointer + sizeof(uint64_t));
  }
  else
  {
    assert(false);
  }
  
}

InternalType ReaderString::getInternalType() const
{
  return m_type;
}

ReaderVector::ReaderVector(const void* attr)
: m_type(InternalType::NULL_)
, m_beginData(nullptr)
, m_size(0)
{
  assert(attr != nullptr);
  m_type = *((InternalType*)attr);
  const unsigned char* sizePointer = (const unsigned char*)attr + sizeof(InternalType);
  
  if(m_type == InternalType::VECTOR8)
  {
    m_size = *((uint8_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint8_t);
  }
  else if(m_type == InternalType::VECTOR16)
  {
    m_size = *((uint16_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint16_t);
  }
  else if(m_type == InternalType::VECTOR32)
  {
    m_size = *((uint32_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint32_t);
  }
  else if(m_type == InternalType::VECTOR64)
  {
    m_size = *((uint64_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }
}

InternalType ReaderVector::getInternalType() const
{
  return m_type;
}

uint64_t ReaderVector::getFullObjectBytesSize() const
{
  uint64_t size = sizeof(InternalType) + m_size;
  
  if(m_type == InternalType::VECTOR8)
  {
    size += sizeof(uint8_t);
  }
  else if(m_type == InternalType::VECTOR16)
  {
    size += sizeof(uint16_t);
  }
  else if(m_type == InternalType::VECTOR32)
  {
    size += sizeof(uint32_t);
  }
  else if(m_type == InternalType::VECTOR64)
  {
    size += sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }
  return size;
}

void ReaderVector::parser() const
{
  const unsigned char* current = m_beginData;
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
: m_type(InternalType::NULL_)
, m_beginData(nullptr)
, m_size(0)
{
  assert(attr != nullptr);
  m_type = *((InternalType*)attr);
  const unsigned char* sizePointer = (const unsigned char*)attr + sizeof(InternalType);

  if(m_type == InternalType::MAP8)
  {
    m_size = *((uint8_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint8_t);
  }
  else if(m_type == InternalType::MAP16)
  {
    m_size = *((uint16_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint16_t);
  }
  else if(m_type == InternalType::MAP32)
  {
    m_size = *((uint32_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint32_t);
  }
  else if(m_type == InternalType::MAP64)
  {
    m_size = *((uint64_t*)sizePointer);
    m_beginData = sizePointer + sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }
  
}

InternalType ReaderMap::getInternalType() const
{
  return m_type;
}

uint64_t ReaderMap::getFullObjectBytesSize() const
{
  uint64_t size = sizeof(InternalType) + m_size;
  
  if(m_type == InternalType::MAP8)
  {
    size += sizeof(uint8_t);
  }
  else if(m_type == InternalType::MAP16)
  {
    size += sizeof(uint16_t);
  }
  else if(m_type == InternalType::MAP32)
  {
    size += sizeof(uint32_t);
  }
  else if(m_type == InternalType::MAP64)
  {
    size += sizeof(uint64_t);
  }
  else
  {
    assert(false);
  }
  return size;
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
  const unsigned char* current = m_beginData;
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
  InternalType* v = (InternalType*)data;
  switch(*v)
  {
    case InternalType::INT8:
    {
      auto value = new ReaderInt8(data);
      *size = sizeof(AttributeValue<InternalType::INT8, int8_t>);
      return value;
    }
    case InternalType::INT16:
    {
      auto value = new ReaderInt16(data);
      *size = sizeof(AttributeValue<InternalType::INT16, int16_t>);
      return value;
    }
    case InternalType::INT32:
    {
      auto value = new ReaderInt32(data);
      *size = sizeof(AttributeValue<InternalType::INT32, int32_t>);
      return value;
    }
    case InternalType::INT64:
    {
      auto value = new ReaderInt64(data);
      *size = sizeof(AttributeValue<InternalType::INT64, int64_t>);
      return value;
    }
    //--
    case InternalType::UINT8:
    {
      auto value = new ReaderUInt8(data);
      *size = sizeof(AttributeValue<InternalType::UINT8, uint8_t>);
      return value;
    }
    case InternalType::UINT16:
    {
      auto value = new ReaderUInt16(data);
      *size = sizeof(AttributeValue<InternalType::UINT16, uint16_t>);
      return value;
    }
    case InternalType::UINT32:
    {
      auto value = new ReaderUInt32(data);
      *size = sizeof(AttributeValue<InternalType::UINT32, uint32_t>);
      return value;
    }
    case InternalType::UINT64:
    {
      auto value = new ReaderUInt64(data);
      *size = sizeof(AttributeValue<InternalType::UINT64, uint64_t>);
      return value;
    }
    case InternalType::FLOAT:
    {
      auto value = new ReaderFloat(data);
      *size = sizeof(AttributeValue<InternalType::FLOAT, float>);
      return value;
    }
    case InternalType::DOUBLE:
    {
      auto value = new ReaderDouble(data);
      *size = sizeof(AttributeValue<InternalType::DOUBLE, double>);
      return value;
    }
    case InternalType::STRING8:
    {
      auto value = new ReaderString(data);
      *size = sizeof(InternalType) + sizeof(uint8_t) + value->length();
      return value;
    }
    case InternalType::STRING16:
    {
      auto value = new ReaderString(data);
      *size = sizeof(InternalType) + sizeof(uint16_t) + value->length();
      return value;
    }
    case InternalType::STRING32:
    {
      auto value = new ReaderString(data);
      *size = sizeof(InternalType) + sizeof(uint32_t) + value->length();
      return value;
    }
    case InternalType::STRING64:
    {
      auto value = new ReaderString(data);
      *size = sizeof(InternalType) + sizeof(uint64_t) + value->length();
      return value;
    }
    case InternalType::VECTOR8:
    case InternalType::VECTOR16:
    case InternalType::VECTOR32:
    case InternalType::VECTOR64:
    {
      auto value = new ReaderVector(data);
      *size = value->getFullObjectBytesSize();
      return value;
    }
   
    case InternalType::MAP8:
    case InternalType::MAP16:
    case InternalType::MAP32:
    case InternalType::MAP64:
    {
      auto value = new ReaderMap(data);
      *size = value->getFullObjectBytesSize();
      return value;
    }
    case InternalType::BOOL:
    {
      auto value = new ReaderBool(data);
      *size = sizeof(AttributeValue<InternalType::BOOL, bool>);
      return value;
    }
    case InternalType::NULL_:
    {
      IReader* value = nullptr;
      *size = sizeof(InternalType);
      return value;
    }
    default:
    return nullptr;
  };
  return nullptr;
}
