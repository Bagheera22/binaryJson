cmake_minimum_required(VERSION  3.1.0)

project(converter)


include_directories(src)
include_directories(lib)

file(GLOB SOURCES "src/binJson/*.cpp")
file(GLOB HEADERS "src/binJson/*.hpp")

set(CMAKE_CXX_STANDARD 11)

add_executable(converter ${SOURCES} ${HEADERS})
