cmake_minimum_required(VERSION 3.6)

project(CasualLibrary)

set(CMAKE_GENERATOR_PLATFORM x64)

include_directories(include)

ADD_LIBRARY( CasualLibrary STATIC
             src/Helper.cpp
             src/Memory.cpp )
