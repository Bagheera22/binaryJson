
#include "Attribute.hpp"
#include "Reader.hpp"
#include "Serialize.hpp"

#include <utility>

#include <cassert>
#include <iostream>
#include "jsonToBin.hpp"

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

/*void print(IReader* data)
{
  switch(data->getType())
  {
    case Type::INT8:
    {
      auto value = (ReaderInt8*)data;
      std::cout<<(int)(value->getValue())<<"\n";
      break;
    }
    case Type::INT16:
    {
      auto value = (ReaderInt16*)data;
      std::cout<<(int)(value->getValue())<<"\n";
      break;
    }
    case Type::INT32:
    {
      auto value = (ReaderInt32*)data;
      std::cout<<(int)(value->getValue())<<"\n";
      break;
    }
    case Type::INT64:
    {
      auto value = (ReaderInt64*)data;
      std::cout<<(long long int)(value->getValue())<<"\n";
      break;
    }
    case Type::UINT8:
    {
      auto value = (ReaderInt8*)data;
      std::cout<<(unsigned int)(value->getValue())<<"\n";
      break;
    }
    case Type::UINT16:
    {
      auto value = (ReaderUInt16*)data;
      std::cout<<(unsigned int)(value->getValue())<<"\n";
      break;
    }
    case Type::UINT32:
    {
      auto value = (ReaderUInt32*)data;
      std::cout<<(unsigned int)(value->getValue())<<"\n";
      break;
    }
    case Type::UINT64:
    {
      auto value = (ReaderUInt32*)data;
      std::cout<<(unsigned long long int)(value->getValue())<<"\n";
      break;
    }
    case Type::FLOAT:
      std::cout<<((ReaderFloat*)data)->getValue()<<"\n";
      break;
    case Type::DOUBLE:
      std::cout<<((ReaderDouble*)data)->getValue()<<"\n";
      break;
    case Type::STRING8:
    case Type::STRING16:
    case Type::STRING32:
    case Type::STRING64:
      std::cout<< ((ReaderString*)data)->getValue() <<"\n";
      break;
    case Type::VECTOR8:
    case Type::VECTOR16:
    case Type::VECTOR32:
    case Type::VECTOR64:
    {
      ReaderVector* v =((ReaderVector*)data);
      std::cout<<"{\n";
      for(size_t i = 0, l = v->size(); i< l ;++i)
      {
        print(v->get((uint64_t)i));
      }
      std::cout<<"}\n";
      break;
    }
    case Type::MAP8:
    case Type::MAP16:
    case Type::MAP32:
    case Type::MAP64:
    {
      ReaderMap* v = ((ReaderMap*)data);

      std::cout<<"{\n";
      for(auto it = v->begin(), end = v->end(); it != end ; ++it)
      {
        std::cout<< "["+it->first+"] ";
        print(it->second);
      }
      std::cout<<"}\n";
      break;
    }
    default:
     std::cout<<"unkonwn type \n";
     break;
  };

}*/




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
  printf("-toBin origin dest\n");
}
