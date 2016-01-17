
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
    InternalType v = obj->getInternalType();
    switch(v)
    {
        case InternalType::INT8:
        {
            const auto value = static_cast<const ReaderInt8*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::INT16:
        {
            const auto value = static_cast<const ReaderInt16*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::INT32:
        {
            const auto value = static_cast<const ReaderInt32*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::INT64:
        {
            const auto value = static_cast<const ReaderInt64*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
            //--
        case InternalType::UINT8:
        {
            const auto value = static_cast<const ReaderUInt8*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::UINT16:
        {
            const auto value = static_cast<const ReaderUInt16*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::UINT32:
        {
            const auto value = static_cast<const ReaderUInt32*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::UINT64:
        {
            const auto value = static_cast<const ReaderUInt64*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::FLOAT:
        {
            const auto value = static_cast<const ReaderFloat*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::DOUBLE:
        {
            const auto value = static_cast<const ReaderDouble*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::STRING8:
        case InternalType::STRING16:
        case InternalType::STRING32:
        case InternalType::STRING64:
        {
            const auto value = static_cast<const ReaderString*>(obj);
            auto valueResult = new rapidjson::GenericValue<Encoding, Allocator>();
            valueResult->SetString(value->getValue().data(), static_cast<rapidjson::SizeType>(value->getValue().length()),allocator);
          return valueResult;
        }
        case InternalType::VECTOR8:
        case InternalType::VECTOR16:
        case InternalType::VECTOR32:
        case InternalType::VECTOR64:
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
            return valueResult;
        }
        case InternalType::MAP8:
        case InternalType::MAP16:
        case InternalType::MAP32:
        case InternalType::MAP64:
        {
            const auto value = static_cast<const ReaderMap*>(obj);
            auto valueResult = new rapidjson::GenericValue<Encoding, Allocator>(rapidjson::Type::kObjectType);
            //valueResult->Reserve((rapidjson::SizeType)value->size(), allocator);
            for (auto it = value->begin() , end = value->end() ; it != end ; ++it)
            {
                auto element = it->second;
                auto elementResult = convert(element,allocator);
                valueResult->AddMember(it->first.c_str(),*elementResult,allocator);
            }
            return valueResult;
        }
        case InternalType::BOOL:
        {
            const auto value = static_cast<const ReaderBool*>(obj);
            return new rapidjson::GenericValue<Encoding, Allocator>(value->getValue());
        }
        case InternalType::NULL_:
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