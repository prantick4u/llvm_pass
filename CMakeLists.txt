cmake_minimum_required(VERSION 3.13)
project(HelloPass)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_VISIBILITY_INLINES_HIDDEN OFF)

add_definitions(-DLLVM_YAML_IS_NEW_PASS_MANAGER)
add_llvm_pass_plugin(HelloPass HelloPass.cpp)