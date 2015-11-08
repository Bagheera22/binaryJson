#include "Serialize.hpp"
#include <cassert>

uint64_t sizeType(ISerialize* v);
void* serializeAttr(ISerialize* v, void* data);



SerializeString::SerializeString( const std::string& data )
: m_data(data)
{
  if (data.size() <=std::numeric_limits<uint8_t>::max() )
  {
    m_type = Type::STRING8;
  }
  else if (data.size() <=std::numeric_limits<uint16_t>::max() )
  {
    m_type = Type::STRING16;
  }
  else if (data.size() <=std::numeric_limits<uint32_t>::max() )
  {
    m_type = Type::STRING32;
  }
  else if (data.size() <=std::numeric_limits<uint64_t>::max() )
  {
    m_type = Type::STRING64;
  }
  else
  {
    assert(false);
  }
}

SerializeString::~SerializeString(){};

void* SerializeString::serialize(void* data) const
{
  data = writeBuffer(data,(void*)&m_type,sizeof(Type));
  if(  m_type == Type::STRING8)
  {
    uint8_t size = (uint8_t)m_data.size();
    data = writeBuffer(data,(void*)&size,sizeof(uint8_t));
  }
  else if(  m_type == Type::STRING16)
  {
    uint16_t size = (uint16_t)m_data.size();
    data = writeBuffer(data,(void*)&size,sizeof(uint16_t));
  }
  else if(  m_type == Type::STRING32)
  {
    uint32_t size = (uint32_t)m_data.size();
    data = writeBuffer(data,(void*)&size,sizeof(uint32_t));
  }
  else if(  m_type == Type::STRING64)
  {
    uint64_t size = (uint64_t)m_data.size();
    data = writeBuffer(data,(void*)&size,sizeof(uint64_t));
  }
  data = writeBuffer(data,(void*)m_data.c_str(),m_data.size()*sizeof(char));
  return data;
}

Type SerializeString::getType() const
{
  return m_type;
}

uint64_t SerializeString::getBytesSize() const
{
  uint64_t size = sizeof(Type) + m_data.size() * sizeof(char);
  if(  m_type == Type::STRING8)
  {
    size += sizeof(uint8_t);
  }
  else if(  m_type == Type::STRING16)
  {
    size += sizeof(uint16_t);
  }
  else if(  m_type == Type::STRING32)
  {
    size += sizeof(uint32_t);
  }
  else if(  m_type == Type::STRING64)
  {
    size += sizeof(uint64_t);
  }
  return size;
}




SerializeVector::SerializeVector(std::vector<ISerialize*>&& value)
: m_value(std::move(value))
{
  uint64_t size = 0;
  for( auto & v : m_value)
  {
    size += sizeType(v);
  }
  m_size = size;
  
  if (size <=std::numeric_limits<uint8_t>::max() )
  {
    m_type = Type::VECTOR8;
  }
  else if (size <=std::numeric_limits<uint16_t>::max() )
  {
    m_type = Type::VECTOR16;
  }
  else if (size <=std::numeric_limits<uint32_t>::max() )
  {
    m_type = Type::VECTOR32;
  }
  else if (size <=std::numeric_limits<uint64_t>::max() )
  {
    m_type = Type::VECTOR64;
  }
  else
  {
    assert(false);
  }
};

SerializeVector::~SerializeVector()
{
  for( auto & v : m_value)
  {
    delete v;
  }
  m_value.clear();
};

uint64_t SerializeVector::getBytesSize() const
{
  uint64_t size = sizeof(Type) + m_size;
  if(  m_type == Type::VECTOR8)
  {
    size += sizeof(uint8_t);
  }
  else if(  m_type == Type::VECTOR16)
  {
    size += sizeof(uint16_t);
  }
  else if(  m_type == Type::VECTOR32)
  {
    size += sizeof(uint32_t);
  }
  else if(  m_type == Type::VECTOR64)
  {
    size += sizeof(uint64_t);
  }
  return size;
}

void* SerializeVector::serialize(void* data) const
{
  data = writeBuffer(data,(void*)&m_type,sizeof(Type));
  if(  m_type == Type::VECTOR8)
  {
    uint8_t size = (uint8_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint8_t));
  }
  else if(  m_type == Type::VECTOR16)
  {
    uint16_t size = (uint16_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint16_t));
  }
  else if(  m_type == Type::VECTOR32)
  {
    uint32_t size = (uint32_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint32_t));
  }
  else if(  m_type == Type::VECTOR64)
  {
    uint64_t size = (uint64_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint64_t));
  }
  for( auto & v : m_value)
  {
    data = serializeAttr(v, data);
  }
  return data;
}

Type SerializeVector::getType() const
{
  return m_type;
}


SerializeMap::SerializeMap(  std::map<std::string,ISerialize*>&& value)
: m_value(std::move(value))
{
  uint64_t size = sizeof(uint16_t) * m_value.size();
  for( auto & v : m_value)
  {
    size += v.first.length() * sizeof(char);
    size += sizeType(v.second);
  }
  m_size = size;
  
  if (size <=std::numeric_limits<uint8_t>::max() )
  {
    m_type = Type::MAP8;
  }
  else if (size <=std::numeric_limits<uint16_t>::max() )
  {
    m_type = Type::MAP16;
  }
  else if (size <=std::numeric_limits<uint32_t>::max() )
  {
    m_type = Type::MAP32;
  }
  else if (size <=std::numeric_limits<uint64_t>::max() )
  {
    m_type = Type::MAP64;
  }
  else
  {
    assert(false);
  }
  
};

SerializeMap::~SerializeMap()
{
  for( auto & v : m_value)
  {
    delete v.second;
  }
  m_value.clear();
};

uint64_t SerializeMap::getBytesSize() const
{
  uint64_t size = sizeof(Type) + m_size;
  if(  m_type == Type::MAP8)
  {
    size += sizeof(uint8_t);
  }
  else if(  m_type == Type::MAP16)
  {
    size += sizeof(uint16_t);
  }
  else if(  m_type == Type::MAP32)
  {
    size += sizeof(uint32_t);
  }
  else if(  m_type == Type::MAP64)
  {
    size += sizeof(uint64_t);
  }
  return size;
}

void* SerializeMap::serialize(void* data) const
{
  data = writeBuffer(data,(void*)&m_type,sizeof(Type));
  if(  m_type == Type::MAP8)
  {
    uint8_t size = (uint8_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint8_t));
  }
  else if(  m_type == Type::MAP16)
  {
    uint16_t size = (uint16_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint16_t));
  }
  else if(  m_type == Type::MAP32)
  {
    uint32_t size = (uint32_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint32_t));
  }
  else if(  m_type == Type::MAP64)
  {
    uint64_t size = (uint64_t)m_size;
    data = writeBuffer(data,(void*)&size,sizeof(uint64_t));
  }
  for( auto & v : m_value)
  {
    uint16_t sizeKey =  static_cast<uint16_t>(v.first.length()*sizeof(char));
    data = writeBuffer(data,(void*)&sizeKey,sizeof(uint16_t));
    data = writeBuffer(data,(void*)v.first.c_str(),sizeKey);
    data = serializeAttr(v.second, data);
  }
  return data;
}

Type SerializeMap::getType() const
{
  return m_type;
}

SerializeNull::SerializeNull()
:m_type(Type::NULL_)
{
}

SerializeNull::~SerializeNull()
{
}

void* SerializeNull::serialize(void* data) const
{
    data = writeBuffer(data,(void*)&m_type,sizeof(Type));
  return data;
}

Type SerializeNull::getType() const
{
  return m_type;
}


uint64_t SerializeNull::getBytesSize() const
{
  return sizeof(Type);
}


void* serializeAttr(ISerialize* v, void* data)
{
switch(v->getType())
{
  case Type::INT8:
    data = ((SerializeInt8*)v)->serialize(data);
    break;
  case Type::INT16:
    data = ((SerializeInt16*)v)->serialize(data);
    break;
  case Type::INT32:
    data = ((SerializeInt32*)v)->serialize(data);
    break;
  case Type::INT64:
    data = ((SerializeInt64*)v)->serialize(data);
    break;
  case Type::UINT8:
    data = ((SerializeUInt8*)v)->serialize(data);
    break;
  case Type::UINT16:
    data = ((SerializeUInt16*)v)->serialize(data);
    break;
  case Type::UINT32:
    data = ((SerializeUInt32*)v)->serialize(data);
    break;
  case Type::UINT64:
    data = ((SerializeUInt64*)v)->serialize(data);
    break;
  case Type::FLOAT:
    data = ((SerializeFloat*)v)->serialize(data);
    break;
  case Type::DOUBLE:
    data = ((SerializeDouble*)v)->serialize(data);
    break;
  case Type::STRING8:
  case Type::STRING16:
  case Type::STRING32:
  case Type::STRING64:
    data = ((SerializeString*)v)->serialize(data);
    break;
  case Type::VECTOR8:
  case Type::VECTOR16:
  case Type::VECTOR32:
  case Type::VECTOR64:
    data = ((SerializeVector*)v)->serialize(data);
    break;
  case Type::MAP8:
  case Type::MAP16:
  case Type::MAP32:
  case Type::MAP64:
    data = ((SerializeMap*)v)->serialize(data);
    break;
  case Type::BOOL:
    data = ((SerializeBool*)v)->serialize(data);
    break;
  case Type::NULL_:
    data = ((SerializeNull*)v)->serialize(data);
    break;
};
  return data;
}

uint64_t sizeType(ISerialize* v)
{
  switch(v->getType())
  {
    case Type::INT8:
      return ((SerializeInt8*)v)->getBytesSize();
    case Type::INT16:
      return ((SerializeInt16*)v)->getBytesSize();
    case Type::INT32:
      return ((SerializeInt32*)v)->getBytesSize();
    case Type::INT64:
      return ((SerializeInt64*)v)->getBytesSize();
    case Type::UINT8:
      return ((SerializeUInt8*)v)->getBytesSize();
    case Type::UINT16:
      return ((SerializeUInt16*)v)->getBytesSize();
    case Type::UINT32:
      return ((SerializeUInt32*)v)->getBytesSize();
    case Type::UINT64:
      return ((SerializeUInt64*)v)->getBytesSize();
    case Type::FLOAT:
      return ((SerializeFloat*)v)->getBytesSize();
    case Type::DOUBLE:
      return ((SerializeDouble*)v)->getBytesSize();
    case Type::STRING8:
    case Type::STRING16:
    case Type::STRING32:
    case Type::STRING64:
      return ((SerializeString*)v)->getBytesSize();
    case Type::VECTOR8:
    case Type::VECTOR16:
    case Type::VECTOR32:
    case Type::VECTOR64:
      return ((SerializeVector*)v)->getBytesSize();
    case Type::MAP8:
    case Type::MAP16:
    case Type::MAP32:
    case Type::MAP64:
      return((SerializeMap*)v)->getBytesSize();
    case Type::BOOL:
      return((SerializeBool*)v)->getBytesSize();
    case Type::NULL_:
      return ((SerializeNull*)v)->getBytesSize();
      break;
  }
}

void* writeBuffer(void* buffer, void *data, size_t len)
{
  memcpy(buffer,data,len);
  return ((unsigned char*)buffer + len);
}
