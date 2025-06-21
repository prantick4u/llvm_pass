
#!/bin/bash
set -e
mkdir -p build
cd build
LLVM_DIR=$(llvm-config --cmakedir)
cmake -DLLVM_DIR=$LLVM_DIR ..
make
