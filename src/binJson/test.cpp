#include <binJson/Attribute.hpp>
#include <binJson/Reader.hpp>
#include <binJson/Serialize.hpp>
#include <binJson/jsonToBin.hpp>
#include <binJson/BinTojson.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <utility>
#include <string.h>
#include <cassert>
#include <iostream>

char* readFile(const std::string& pathname)
{
    FILE* f = fopen(pathname.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* string = (char*)malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    return string;
}

bool testJsonToBinToJson(const char* jsonTxt)
{
    binJson::ISerialize* bin = binJson::convertJsonToBinJson(jsonTxt);

    uint64_t sizeBin = bin->getBytesSize();
    void* dataBin = malloc(sizeBin);
    bin->serialize(dataBin);

    binJson::rapidjsonValue_ptr json = binJson::convertBinJsonToJson((const char*)dataBin);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json->Accept(writer);

    std::string jsonResult(buffer.GetString(), buffer.Size());
    bool ok = (strcmp(jsonResult.c_str(), jsonTxt) == 0);
    assert(ok);
    return ok;
}

int main(int argc, char* argv[])
{
    testJsonToBinToJson("{\"a\":1}");
    testJsonToBinToJson("{\"a\":true}");
    testJsonToBinToJson("{\"a\":1233.03}");
    testJsonToBinToJson("{\"a\":\"adasd\"}");
    testJsonToBinToJson("{\"a\":[]}");
    testJsonToBinToJson("{\"a\":{}}");
    testJsonToBinToJson("{\"a\":{\"a\":2}}");

    if (argc == 4 && strcmp(argv[1], "-toBin") == 0)
    {

        auto dataJson = readFile(argv[2]);
        binJson::ISerialize* bin = binJson::convertJsonToBinJson(dataJson);

        uint64_t sizeBin = bin->getBytesSize();
        void* dataBin = malloc(sizeBin);
        bin->serialize(dataBin);

        FILE* f = fopen(argv[3], "w");
        fwrite(dataBin, sizeof(char), sizeBin, f);
        fclose(f);
        free(dataBin);
        return 0;
    }
    if (argc == 4 && strcmp(argv[1], "-toJson") == 0)
    {
        auto dataBin = readFile(argv[2]);
        binJson::rapidjsonValue_ptr json = binJson::convertBinJsonToJson(dataBin);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        json->Accept(writer);
        FILE* f = fopen(argv[3], "w");
        fwrite(buffer.GetString(), sizeof(char), strlen(buffer.GetString()), f);
        fclose(f);

        return 0;
    }
    printf("-toBin|-toJson origin dest\n");
}
