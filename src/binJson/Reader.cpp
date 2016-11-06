#include <binJson/Reader.hpp>
#include <iostream>
#include <cassert>
#include <array>
#include <functional>
namespace binJson
{
    namespace internal
    {
        const std::array<std::function<void(const unsigned char* sizePtr, uint64_t& size, const unsigned char*& beginData)>,4> calcSize
        ={
            [](const unsigned char* sizePtr, uint64_t& size, const unsigned char*& beginData )
            {
                size = *((uint8_t*)sizePtr);
                beginData = sizePtr + sizeof(uint8_t);
            },
            [](const unsigned char* sizePtr, uint64_t& size, const unsigned char*& beginData )
            {
                size = *((uint16_t*)sizePtr);
                beginData = sizePtr + sizeof(uint16_t);
            },
            [](const unsigned char* sizePtr, uint64_t& size, const unsigned char*& beginData )
            {
                size = *((uint32_t*)sizePtr);
                beginData = sizePtr + sizeof(uint32_t);
            },
            [](const unsigned char* sizePtr, uint64_t& size, const unsigned char*& beginData )
            {
                size = *((uint64_t*)sizePtr);
                beginData = sizePtr + sizeof(uint64_t);
            }
        };
        /*
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
 */
        const std::function<IReader*(const void* data, uint64_t* size)> readStrSize
        = [](const void* data, uint64_t* size)
        {
            auto value = new ReaderString(data);
            *size = value->getFullObjectBytesSize();
            return value;
        };

        const std::function<IReader*(const void* data, uint64_t* size)> readVecSize
        = [](const void* data, uint64_t* size)
        {
            auto value = new ReaderVector(data);
            *size = value->getFullObjectBytesSize();
            return value;
        };

        const std::function<IReader*(const void* data, uint64_t* size)> readMapSize
        = [](const void* data, uint64_t* size)
        {
            auto value = new ReaderMap(data);
            *size = value->getFullObjectBytesSize();
            return value;
        };        

        const std::array<std::function<IReader*(const void* data, uint64_t* size)>,(int)InternalType::STRING64+1> readSize
        = {
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(InternalType);
                return nullptr;
            },
            //int
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT8, int8_t>);
                return new ReaderInt8(data);
            },
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT16, int16_t>);
                return new ReaderInt16(data);
            },
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT32, int32_t>);
                return new ReaderInt32(data);
            },
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT64, int64_t>);
                return new ReaderInt64(data);
            },
            //uint
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT8, uint8_t>);
                return new ReaderUInt8(data);
            },
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT16, uint16_t>);
                return new ReaderUInt16(data);
            },
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT32, uint32_t>);
                return new ReaderUInt32(data);
            },
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::INT64, uint64_t>);
                return new ReaderUInt64(data);
            },   
            //FLOAT
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::FLOAT, float>);
                return new ReaderFloat(data);
            },
            //DOUBLE
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::DOUBLE, double>);
                return new ReaderDouble(data);
            },
            //BOOL
            [](const void* data, uint64_t* size)
            {
                *size = sizeof(AttributeValue<InternalType::BOOL, bool>);
                return new ReaderBool(data);
            },
            readVecSize,
            readVecSize,
            readVecSize,
            readVecSize,

            readMapSize,
            readMapSize,
            readMapSize,
            readMapSize,
    
            readStrSize,
            readStrSize,
            readStrSize,
            readStrSize
        };
    }
ReaderString::ReaderString(const void* valueAttr)
    : m_type(InternalType::NULL_)
    , m_size(0)
{
    assert(valueAttr != nullptr);
    m_type = *((InternalType*)valueAttr);
    const unsigned char* sizePointer = (const unsigned char*)valueAttr + sizeof(InternalType);
    const unsigned char* temp = nullptr;
    internal::calcSize.at((int)m_type-(int)InternalType::STRING8)(sizePointer, m_size,temp );
    m_str = (const char*) temp;
}

InternalType ReaderString::getInternalType() const
{
    return m_type;
}

uint64_t ReaderString::getFullObjectBytesSize() const
{
    uint64_t size = sizeof(InternalType) + m_size;

    uint64_t sizes[4] = { sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t) };
    int index = ((int)m_type - (int)InternalType::STRING8);
    assert(index >= 0 && index < 4);
    size += sizes[index];
    return size;
}

ReaderVector::ReaderVector(const void* attr)
    : m_type(InternalType::NULL_)
    , m_beginData(nullptr)
    , m_size(0)
{
    assert(attr != nullptr);
    m_type = *((InternalType*)attr);
    const unsigned char* sizePointer = (const unsigned char*)attr + sizeof(InternalType);
    internal::calcSize.at((int)m_type-(int)InternalType::VECTOR8)(sizePointer, m_size,m_beginData );
}

InternalType ReaderVector::getInternalType() const
{
    return m_type;
}

uint64_t ReaderVector::getFullObjectBytesSize() const
{
    uint64_t size = sizeof(InternalType) + m_size;

    uint64_t sizes[4] = { sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t) };
    int index = ((int)m_type - (int)InternalType::VECTOR8);
    assert(index >= 0 && index < 4);
    size += sizes[index];
    return size;
}

void ReaderVector::parser() const
{
    const unsigned char* current = m_beginData;
    const unsigned char* end = current + m_size;

    while (current < end)
    {
        uint64_t size = 0;
        IReader* elemment = read(current, &size);
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
    internal::calcSize.at((int)m_type-(int)InternalType::MAP8)(sizePointer, m_size,m_beginData );
}

InternalType ReaderMap::getInternalType() const
{
    return m_type;
}

uint64_t ReaderMap::getFullObjectBytesSize() const
{
    uint64_t size = sizeof(InternalType) + m_size;

    uint64_t sizes[4] = { sizeof(uint8_t), sizeof(uint16_t), sizeof(uint32_t), sizeof(uint64_t) };
    int index = ((int)m_type - (int)InternalType::MAP8);
    assert(index >= 0 && index < 4);
    size += sizes[index];
    return size;
}

std::map<std::string, IReader*>::const_iterator ReaderMap::begin() const
{
    if (m_elements.empty())
    {
        parser();
    }
    return m_elements.begin();
}

std::map<std::string, IReader*>::const_iterator ReaderMap::end() const
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
    const unsigned char* end = current + m_size;

    while (current < end)
    {
        // ReaderString
        uint16_t* lengthStringKey = (uint16_t*)current;
        current += sizeof(uint16_t);
        std::string key((const char*)current, *lengthStringKey);
        current += *lengthStringKey;

        // read value
        uint64_t size = 0;
        IReader* elemment = read(current, &size);

        m_elements.insert(std::make_pair(key, elemment));
        current += size;
    }
}

IReader* read(const void* data, uint64_t* size)
{
    InternalType* v = (InternalType*)data;
    auto func = internal::readSize.at((int)*v);
    return func(data, size);
}
};
