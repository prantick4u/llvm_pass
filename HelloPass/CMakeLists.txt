cmake_minimum_required(VERSION 3.13)
project(HelloPass)

# Find LLVM and enable C++17
find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Define the pass as a plugin for the new pass manager
add_llvm_pass_plugin(HelloPass HelloPass.cpp)

# Ensure symbols like llvmGetPassPluginInfo are exported
set_target_properties(HelloPass PROPERTIES
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)