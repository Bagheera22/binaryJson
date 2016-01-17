#ifndef _BINJSON_READER_H_
#define _BINJSON_READER_H_

#include <cstdint>
#include "Attribute.hpp"

class IReader
{
  public:
  virtual ~IReader() = default;
    virtual InternalType getInternalType() const = 0;
    virtual Type getType() const = 0;
};

IReader* read(const void* data, uint64_t* size );

template<InternalType type, typename Value>
class ReaderValue : public IReader
{
private:
  const AttributeValue<type,Value>* m_valueAttr;
public:
  ReaderValue(const void* valueAttr)
  :m_valueAttr((AttributeValue<type,Value>*)valueAttr)
  {
  }
  inline const Value& getValue() const
  {
    return m_valueAttr->getValue();
  }
  InternalType getInternalType() const override
  {
    return m_valueAttr->getType();
  }
  Type getType() const override
  {
    return static_cast<Type>( m_valueAttr->getType());
  }
};


class ReaderString : public IReader
{
private:
  InternalType m_type;
  mutable const char* m_str;
  mutable uint64_t m_size;
public:
  ReaderString(const void* valueAttr);

  inline std::string getValue() const
  {
    std::string value(m_str,(size_t)m_size);
    return std::move(value);
  }
  InternalType getInternalType() const override;
  
  Type getType() const override
  {
    return Type::STRING;
  }

  inline uint64_t length()
  {
    return m_size;
  }

};

class ReaderVector : public IReader
{
private:
  InternalType m_type;
  const unsigned char* m_beginData;
  mutable uint64_t m_size;
  mutable std::vector<IReader*> m_elements;
public:
  ReaderVector(const void* attr);

  inline IReader* get(const uint64_t i) const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_elements.at(i);
  }
  inline size_t size() const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_elements.size();
  }
  InternalType getInternalType() const override;
  
  Type getType() const override
  {
    return Type::VECTOR;
  }

  inline uint64_t getContentBytesSize() const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_size;
  }
  uint64_t getFullObjectBytesSize() const;
  

  std::vector<IReader*>::const_iterator begin() const;

  std::vector<IReader*>::const_iterator end() const;
private:
  void parser() const;
};


class ReaderMap : public IReader
{
private:
  InternalType m_type;
  const unsigned char* m_beginData;
  mutable uint64_t m_size;
  mutable std::map<std::string,IReader*> m_elements;
public:
  ReaderMap(const void* attr);

  inline IReader* get(const std::string& i) const
  {
    if (m_elements.empty())
    {
      parser();
    }
    auto it = m_elements.find(i);
    if( it!= m_elements.end())
    {
      return it->second;
    }
    return nullptr;
  }
  inline size_t size() const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_elements.size();
  }
  InternalType getInternalType() const override;
  
  Type getType() const override
  {
    return Type::MAP;
  }

  inline uint64_t getContentBytesSize() const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_size;
  }

  uint64_t getFullObjectBytesSize() const;

  std::map<std::string,IReader*>::const_iterator begin() const;

  std::map<std::string,IReader*>::const_iterator end() const;

private:
    void parser() const;
};

using ReaderInt8 = ReaderValue<InternalType::INT8, int8_t>;
using ReaderInt16 = ReaderValue<InternalType::INT16, int16_t>;
using ReaderInt32 = ReaderValue<InternalType::INT32, int32_t>;
using ReaderInt64 = ReaderValue<InternalType::INT64, int64_t>;

using ReaderUInt8 = ReaderValue<InternalType::UINT8, uint8_t>;
using ReaderUInt16 = ReaderValue<InternalType::UINT16, uint16_t>;
using ReaderUInt32 = ReaderValue<InternalType::UINT32, uint32_t>;
using ReaderUInt64 = ReaderValue<InternalType::UINT64, uint64_t>;

using ReaderFloat = ReaderValue<InternalType::FLOAT, float>;
using ReaderDouble = ReaderValue<InternalType::DOUBLE, double>;
using ReaderBool = ReaderValue<InternalType::BOOL, bool>;

#endif
