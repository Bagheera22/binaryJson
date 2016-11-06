#include <binJson/jsonToBin.hpp>
#include <binJson/Serialize.hpp>
#include <binJson/rapidjsonConstants.hpp>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <limits>

namespace binJson
{

ISerialize* convertBin(const rapidjsonValue& json)
{

    if (json.IsObject())
    {
        std::map<std::string, ISerialize*> map;
        for (rapidjsonValue::ConstMemberIterator itr = json.MemberBegin(), end = json.MemberEnd(); itr != end; ++itr)
        {
            map.insert(std::make_pair(itr->name.GetString(), convertBin(itr->value)));
        }
        return new SerializeMap(std::move(map));
    }

    if (json.IsArray())
    {
        std::vector<ISerialize*> vector;
        vector.reserve(json.Size());
        for (::rapidjson::SizeType i = 0, size = json.Size(); i < size; i++)
        {
            vector.push_back(convertBin(json[i]));
        }
        return new SerializeVector(std::move(vector));
    }
    if (json.IsNull())
    {
        return new SerializeNull();
    }
    if (json.IsBool())
    {
        return new SerializeBool(json.GetBool());
    }
    if (json.IsInt())
    { //https://msdn.microsoft.com/en-us/library/7fh3a000.aspx
        int32_t value = json.GetInt();

        if (value >= std::numeric_limits<int8_t>::min() && value <= std::numeric_limits<int8_t>::max())
        {
            return new SerializeInt8(value);
        }
        if (value >= std::numeric_limits<int16_t>::min() && value <= std::numeric_limits<int16_t>::max())
        {
            return new SerializeInt16(value);
        }

        return new SerializeInt32(std::move(value));
    }
    if (json.IsUint())
    {
        uint32_t value = json.GetUint();
        if (value <= std::numeric_limits<uint8_t>::max())
        {
            return new SerializeInt8(value);
        }
        if (value <= std::numeric_limits<uint16_t>::max())
        {
            return new SerializeInt16(value);
        }

        return new SerializeUInt32(std::move(value));
    }
    if (json.IsInt64())
    {
        return new SerializeInt64(json.GetInt64());
    }
    if (json.IsUint64())
    {
        return new SerializeUInt64(json.GetInt64());
    }
    if (json.IsString())
    {
        return new SerializeString(json.GetString());
    }
    if (json.IsDouble())
    {
        double d = json.GetDouble();
        float p = (float)d;
        if (d == (double)p)
        {
            return new SerializeFloat(std::move(p));
        }
        return new SerializeDouble(std::move(d));
    }
    return nullptr;
}

ISerialize* convertJsonToBinJson(const char* data)
{
    rapidjsonGenericDocument doc;
    if (doc.Parse<0>(data).HasParseError())
    {
        return nullptr;
    }
    if (doc.IsArray())
    {
        std::vector<ISerialize*> vector;
        vector.reserve(doc.Size());
        for (rapidjson::SizeType size = doc.Size(), i = 0; i < size; i++)
        {
            vector.push_back(convertBin(doc[i]));
        }
        return new SerializeVector(vector);
    }
    if (doc.IsObject())
    {
        std::map<std::string, ISerialize*> map;
        for (rapidjsonValue::ConstMemberIterator itr = doc.MemberBegin(), end = doc.MemberEnd(); itr != end; ++itr)
        {
            map.insert(std::make_pair(itr->name.GetString(), convertBin(itr->value)));
        }
        return new SerializeMap(map);
    }
    return nullptr;
}
}
