cmake_minimum_required(VERSION 3.20)

project(sdinsight VERSION 0.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
endif()


file(GLOB src_files
     src/*.cpp)


add_definitions(-fno-omit-frame-pointer)
add_executable(sdinsight ${src_files})
target_include_directories(sdinsight PRIVATE third_party/ third_party/magic_enum-v0.9.6/include/ src/)
target_link_libraries(sdinsight PRIVATE stdc++_libbacktrace)
target_link_options(sdinsight PRIVATE -static-libgcc -static-libstdc++ -static)
