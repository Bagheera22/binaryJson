#ifndef _BINJSON_READER_H_
#define _BINJSON_READER_H_

#include <cstdint>
#include "Attribute.hpp"

class IReader
{
  public:
    virtual Type getType() const = 0;
};

IReader* read(const void* data, uint64_t* size );

template<Type type, typename Value>
class ReaderValue : public IReader
{
private:
  const AttributeValue<type,Value>* m_valueAttr;
public:
  ReaderValue(const void* valueAttr)
  :m_valueAttr((AttributeValue<type,Value>*)valueAttr)
  {
  }
  inline Value getValue() const
  {
    return m_valueAttr->getValue();
  }
  Type getType() const override
  {
    return m_valueAttr->getType();
  }
};


class ReaderString : public IReader
{
private:
  const Type* m_type;
  mutable const char* m_str;
  mutable uint64_t m_size;
public:
  ReaderString(const void* valueAttr);

  inline std::string getValue() const
  {
    if (m_size == 0)
    {
      parser();
    }
    std::string value(m_str,(size_t)m_size);
    return std::move(value);
  }
  Type getType() const override;

  inline uint64_t length()
  {
    if (m_size == 0)
    {
      parser();
    }
    return m_size;
  }
private:
  void parser() const;

};

class ReaderVector : public IReader
{
private:
  const Type* m_type;
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
  Type getType() const override;

  inline uint64_t getBytesSize() const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_size;
  }

  std::vector<IReader*>::const_iterator begin() const;

  std::vector<IReader*>::const_iterator end() const;
private:
  void parser() const;
};


class ReaderMap : public IReader
{
private:
  const Type* m_type;
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
  Type getType() const override;

  inline uint64_t getBytesSize() const
  {
    if (m_elements.empty())
    {
      parser();
    }
    return m_size;
  }

  std::map<std::string,IReader*>::const_iterator begin() const;

  std::map<std::string,IReader*>::const_iterator end() const;

private:
    void parser() const;
};

using ReaderInt8 = ReaderValue<Type::INT8, int8_t>;
using ReaderInt16 = ReaderValue<Type::INT16, int16_t>;
using ReaderInt32 = ReaderValue<Type::INT32, int32_t>;
using ReaderInt64 = ReaderValue<Type::INT64, int64_t>;

using ReaderUInt8 = ReaderValue<Type::UINT8, uint8_t>;
using ReaderUInt16 = ReaderValue<Type::UINT16, uint16_t>;
using ReaderUInt32 = ReaderValue<Type::UINT32, uint32_t>;
using ReaderUInt64 = ReaderValue<Type::UINT64, uint64_t>;

using ReaderFloat = ReaderValue<Type::FLOAT, float>;
using ReaderDouble = ReaderValue<Type::DOUBLE, double>;
using ReaderBool = ReaderValue<Type::BOOL, bool>;

#endif
