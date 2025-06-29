cmake_minimum_required(VERSION 3.13)
project(SecurityPass)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_library(SecurityPass MODULE HelloSecurityPass.cpp)

set_target_properties(SecurityPass PROPERTIES
  COMPILE_FLAGS "-fno-rtti"
)
