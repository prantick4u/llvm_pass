cmake_minimum_required(VERSION 3.13)
project(HelloPass)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add your pass as a MODULE so it becomes a loadable .so plugin
add_library(HelloPass MODULE HelloPass.cpp)

# Required for legacy pass plugins
set_target_properties(HelloPass PROPERTIES
  COMPILE_FLAGS "-fno-rtti"
)
