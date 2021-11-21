cmake_minimum_required(VERSION 3.6)

project(CasualLibrary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(Library)

set(SOURCES Library/CasualLibrary.hpp
            Library/CasualLibrary/Helper.hpp
            Library/CasualLibrary/Helper.cpp
            Library/CasualLibrary/Address.hpp
            Library/CasualLibrary/Address.cpp
	    Library/CasualLibrary/Macros.hpp
            Library/CasualLibrary/Memory/External.hpp
            Library/CasualLibrary/Memory/External.cpp 
            Library/CasualLibrary/Memory/Internal.hpp
            Library/CasualLibrary/Memory/Internal.cpp
            Library/CasualLibrary/Memory/Memory.hpp
            Library/CasualLibrary/Memory/Memory.cpp
)


ADD_LIBRARY(CasualLibrary STATIC ${SOURCES})