#pragma once
#include <memory>


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

namespace binJson
{
	typedef rapidjson::UTF8<char> EncodingJson;
	typedef rapidjson::CrtAllocator Allocator;
	using rapidjsonValue = rapidjson::GenericValue<EncodingJson, Allocator>;
	using rapidjsonGenericDocument= rapidjson::GenericDocument<EncodingJson, Allocator>;
	using rapidjsonValue_ptr = std::unique_ptr<rapidjsonValue>;
}