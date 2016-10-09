cmake_minimum_required(VERSION  2.8.7)

project(converter)


include_directories(src)
include_directories(lib)

file(GLOB SOURCES "src/binJson/*.cpp")
file(GLOB HEADERS "src/binJson/*.hpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(converter ${SOURCES} ${HEADERS})
