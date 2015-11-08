
#include "Attribute.hpp"
#include "Reader.hpp"
#include "Serialize.hpp"
#include "stringbuffer.h"
#include "writer.h"
#include <utility>

#include <cassert>
#include <iostream>
#include "jsonToBin.hpp"
#include "BinToJson.hpp"

char* readFile(const std::string& pathname)
{
    FILE *f = fopen(pathname.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *string = (char *)malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    return string;
}

int main(int argc, char* argv[])
{
  if( argc == 4 && strcmp(argv[1],"toBin") == 0 )
  {

    auto dataJson = readFile(argv[2]);
    ISerialize* bin = jsonToBin(dataJson);
    
    uint64_t sizeBin = bin->getBytesSize();
    void*  dataBin= malloc(sizeBin);
    bin->serialize(dataBin);

    FILE* f = fopen(argv[3],"w");
    fwrite(dataBin , sizeof(char), sizeBin,f);
    fclose(f);
    
    return 0;
  }
  if( argc == 4 && strcmp(argv[1],"toJson") == 0 )
  {
    auto dataBin = readFile(argv[2]);
    auto json = binToJson(dataBin);
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json->Accept(writer);
    delete json;
    FILE* f = fopen(argv[3],"w");
    fwrite(buffer.GetString() , sizeof(char), strlen(buffer.GetString()),f);
    fclose(f);
    
    return 0;
  }
  printf("-toBin origin dest\n");
}
