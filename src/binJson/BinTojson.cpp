
#include <binJson/BinTojson.hpp>
#include <binJson/Reader.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <limits>
#include <array>
#include <functional>

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
using namespace rapidjson;
namespace binJson
{
    rapidjsonValue_ptr convert(const IReader* obj, Allocator& allocator);
    namespace internal
    {
            const std::function<rapidjsonValue_ptr(const IReader* obj,Allocator& allocator)>vectorConverter
            = [](const IReader* obj,Allocator& allocator)
            {
                const auto value = static_cast<const ReaderVector*>(obj);
                auto valueResult = rapidjsonValue_ptr(new rapidjsonValue(rapidjson::Type::kArrayType));
                valueResult->Reserve((rapidjson::SizeType)value->size(), allocator);
                for (auto it = value->begin(), end = value->end(); it != end; ++it)
                {
                    auto element = *it;
                    auto elementResult = convert(element, allocator);
                    valueResult->PushBack(*elementResult, allocator);
                }
                return valueResult;
            };
        
        
        const std::function<rapidjsonValue_ptr(const IReader* obj,Allocator& allocator)>mapConverter
        = [](const IReader* obj,Allocator& allocator)
        {
            const auto value = static_cast<const ReaderMap*>(obj);
            auto valueResult = rapidjsonValue_ptr(new rapidjsonValue(rapidjson::Type::kObjectType));
            //valueResult->Reserve((rapidjson::SizeType)value->size(), allocator);
            for (auto it = value->begin(), end = value->end(); it != end; ++it)
            {
                auto element = it->second;
                auto elementResult = convert(element, allocator);
                valueResult->AddMember(it->first.c_str(), *elementResult, allocator);
            }
            return valueResult;
        };
        
        const std::function<rapidjsonValue_ptr(const IReader* obj,Allocator& allocator)> stringConverter
        = [](const IReader* obj,Allocator& allocator)
        {
            const auto value = static_cast<const ReaderString*>(obj);
            auto valueResult = rapidjsonValue_ptr(new rapidjsonValue());
            valueResult->SetString(value->getValue().data(), static_cast<rapidjson::SizeType>(value->getValue().length()), allocator);
            return valueResult;
        };
        
    
        const std::array<std::function<rapidjsonValue_ptr(const IReader* obj,Allocator& allocator)>,(int)InternalType::STRING64+1> typeConverter =
        {
            [](const IReader* /*obj*/,Allocator& /*allocator*/)
            {
                auto value = rapidjsonValue_ptr(new rapidjsonValue());
                value->SetNull();
                return value;
            },
            //ints
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderInt8*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            }
            ,
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderInt16*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            }
            ,
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderInt32*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            }
            ,
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderInt64*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            },
            //uints
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderUInt8*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            }
            ,
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderUInt16*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            }
            ,
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderUInt32*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            }
            ,
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderUInt64*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            },
            //float
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderFloat*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            },
            //double
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderDouble*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            },
            //bool
            [](const IReader* obj,Allocator& /*allocator*/)
            {
                const auto value = static_cast<const ReaderBool*>(obj);
                return rapidjsonValue_ptr(new rapidjsonValue(value->getValue()));
            },
            //vector
            vectorConverter,
            vectorConverter,
            vectorConverter,
            vectorConverter,
            //map
            mapConverter,
            mapConverter,
            mapConverter,
            mapConverter,
            //string
            stringConverter,
            stringConverter,
            stringConverter,
            stringConverter
        };
    }
    
rapidjsonValue_ptr convert(const IReader* obj, Allocator& allocator)
{
   InternalType v = obj->getInternalType();
   return internal::typeConverter.at((int)v)(obj,allocator);
}

rapidjsonValue_ptr convertBinJsonToJson(const char* data)
{
    Allocator allocator;
    uint64_t size = 0;
    IReader* obj = read(data, &size);
    return convert(obj, allocator);
}
}
