# 1. Delete previous build
rm -rf build

# 2. Recreate and reconfigure
mkdir build
cd build
cmake ..

# 3. Rebuild the pass
make
