When you compile with -O1 or higher, Clang may optimize away mul instructions during IR generation.
That’s why:


-O0 is useful for preserving original operations like mul.

-But -O0 adds optnone, which disables all LLVM opt passes — including your custom one.


clang -S -emit-llvm ./test/test.c -o test.ll -Xclang -disable-O0-optnone -O0



opt -load-pass-plugin build/ReplaceMultWithAdd.dylib -debug-pass-manager \  
    -passes=replace-mult-with-add test.ll -S -o output.ll