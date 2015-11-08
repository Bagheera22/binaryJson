#ifndef BinTojson_hpp
#define BinTojson_hpp
#include "rapidjson/document.h"

typedef rapidjson::CrtAllocator Allocator;
typedef rapidjson::UTF8<char> Encoding;
rapidjson::GenericValue<Encoding, Allocator>* binToJson(const char* data);

#endif /* jsonToBin_hpp */
