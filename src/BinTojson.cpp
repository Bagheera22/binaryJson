
#include "BinTojson.hpp"
#include "Reader.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
namespace rapidjson
{
    class CrtAllocator;
    template <typename CharType>
    struct UTF8;
    template <typename BaseAllocator>
    class MemoryPoolAllocator;
    template <typename Encoding, typename Allocator>
    class GenericValue;
    template <typename Encoding, typename Allocator>
    class GenericDocument;
};

typedef rapidjson::UTF8<char> Encoding;
typedef rapidjson::CrtAllocator Allocator;
//typedef rapidjson::GenericValue<Encoding, Allocator> Value;


rapidjson::GenericValue<Encoding, Allocator>* convert(const IReader* obj, Allocator& allocator)
{
    Type v = obj->getType();
    switch(v)
    {
        case Type::INT8:
        {
            const auto value = static_cast<const ReaderInt8*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::INT16:
        {
            const auto value = static_cast<const ReaderInt16*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::INT32:
        {
            const auto value = static_cast<const ReaderInt32*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::INT64:
        {
            const auto value = static_cast<const ReaderInt64*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
            //--
        case Type::UINT8:
        {
            const auto value = static_cast<const ReaderUInt8*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::UINT16:
        {
            const auto value = static_cast<const ReaderUInt16*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::UINT32:
        {
            const auto value = static_cast<const ReaderUInt32*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::UINT64:
        {
            const auto value = static_cast<const ReaderUInt64*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::FLOAT:
        {
            const auto value = static_cast<const ReaderFloat*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::DOUBLE:
        {
            const auto value = static_cast<const ReaderDouble*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::STRING8:
        case Type::STRING16:
        case Type::STRING32:
        case Type::STRING64:
        {
            const auto value = static_cast<const ReaderString*>(obj);
            auto valueResult = new rapidjson::GenericValue<Encoding, Allocator>();
            valueResult->SetString(value->getValue().data(), static_cast<rapidjson::SizeType>(value->getValue().length()),allocator);
        }
        case Type::VECTOR8:
        case Type::VECTOR16:
        case Type::VECTOR32:
        case Type::VECTOR64:
        {
            const auto value = static_cast<const ReaderVector*>(obj);
            auto valueResult = new rapidjson::GenericValue<Encoding, Allocator>(rapidjson::Type::kArrayType);
            valueResult->Reserve((rapidjson::SizeType)value->size(), allocator);
            for (auto it = value->begin() , end = value->end() ; it != end ; ++it)
            {
                auto element = *it;
                auto elementResult = convert(element,allocator);
                valueResult->PushBack(*elementResult, allocator);
            }
            
        }
        case Type::MAP8:
        case Type::MAP16:
        case Type::MAP32:
        case Type::MAP64:
        {
            const auto value = static_cast<const ReaderMap*>(obj);
            auto valueResult = new rapidjson::GenericValue<Encoding, Allocator>(rapidjson::Type::kObjectType);
            valueResult->Reserve((rapidjson::SizeType)value->size(), allocator);
            for (auto it = value->begin() , end = value->end() ; it != end ; ++it)
            {
                auto element = it->second;
                auto elementResult = convert(element,allocator);
                valueResult->AddMember(it->first.c_str(),*elementResult,allocator);
            }
        }
        case Type::BOOL:
        {
            const auto value = static_cast<const ReaderBool*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case Type::NULL_:
        {
            auto value = new rapidjson::GenericValue<Encoding, Allocator>();
            value->SetNull();
            return value;
        }
        default:
            return nullptr;
    };
    return nullptr;
}

rapidjson::GenericValue<Encoding, Allocator>* binToJson(const char* data)
{
    Allocator allocator;
    uint64_t size = 0;
    IReader*  obj = read(data, &size);
    return convert(obj,allocator);
}