#!/bin/bash

rm -rf build
mkdir build
cd build

# Explicitly point to Homebrew LLVM config directory
cmake .. \
  -DLLVM_DIR=/opt/homebrew/opt/llvm/lib/cmake/llvm \
  -DCMAKE_BUILD_TYPE=Release

make

