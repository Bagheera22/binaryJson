
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

void print(IReader* data)
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

}


void Serialize(SerializeVector* root)
{
  uint64_t size = root->getBytesSize();
  std::cout <<"size: "<<size<<"\n";
  void* m = malloc(size);
  root->serialize(m);
  std::cout <<"serialize\n";
  FILE* f = fopen("a.binJson","w");
  fwrite(m , sizeof(char), size,f);
  fclose(f);

  //Reader reader;
  //IAttribute* xx = reader.read(m);
  //print(xx);
  uint64_t sizel = 0;
  auto a = getReader(m, &sizel );
  print(a);
}

int main()
{
  // test1
  std::vector<ISerialize*> test1;
  test1.push_back(new SerializeInt8(101));
  test1.push_back(new SerializeInt16(201));
  test1.push_back(new SerializeInt8(30));
  test1.push_back(new SerializeInt8(40));


  test1.push_back(new SerializeInt8(101));
  test1.push_back(new SerializeInt32(201));
  test1.push_back(new SerializeInt64(30));
  test1.push_back(new SerializeFloat(40.02));

  test1.push_back(new SerializeString("guapaaaaaaaa"));

  std::map<std::string, ISerialize*> map;
  map.insert(std::make_pair("aoo",new SerializeInt8(10)));
  test1.push_back(new SerializeMap(std::move(map)));

  //SerializeVector*  root =  new SerializeVector(std::move(test1));
  //Serialize(root);
  //*
    auto a = readFile("/Volumes/UUI/binjson/data.json");
    ISerialize* b = jsonToBin(a);
    
    uint64_t size = b->getBytesSize();
    void* m = malloc(size);
    b->serialize(m);
    FILE* f = fopen("/Volumes/UUI/binjson/data.binJson","w");
    fwrite(m , sizeof(char), size,f);
    fclose(f);
    
    uint64_t sizel = 0;
    print(getReader(m, &sizel ));
  //*/
    
}
