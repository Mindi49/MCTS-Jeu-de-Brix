cmake_minimum_required(VERSION 2.8)
project(TreesFunctions CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wpedantic)
endif()

add_executable(${PROJECT_NAME} main.cc tree.hh node.hh value.hh treeutil.cc)
