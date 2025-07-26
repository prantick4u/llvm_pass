This pass identifies all the memory allocation store and load instruction and wraps an instrumentation around them to identify clearly.

First we need to compile the pass using the build.sh command. Then we can follow the steps below to verify the run.

1. ✅ Run your LLVM Pass

    clang -O0 -g -S -emit-llvm test/test.c -o test.ll -Xclang -disable-O0-optnone

    -O0: keeps instructions (no optimization removes debug info).

    -g: emits DWARF debug info (including file and line).

    -S -emit-llvm: generates readable .ll IR.

    Note: We need to remove the optnone from the test.ll file using the flag `-Xclang -disable-O0-optnone`, otherwise the functions will be skipped.

    opt -load-pass-plugin ./build/MemoryInstrumentation.dylib -passes=mem-instrument < test.ll > instrumented.ll

    Now instrumented.ll includes `call void @__log_access(i8*)` before every load/store.


2. 🧱 Compile the logger function
    
    clang -c logger/logger.c -o logger.o   

    This creates logger.o with your __log_access implementation.

3. 🔗 Link both to create final executable
    
    clang instrumented.ll logger.o -o runme.exe

4. 🏃 Run it
    
    ./runme.exe

The output would be like:

🔍 Store of 4 bytes at 0x16f8134dc in main (compiler-inserted:0)

🔍 Store of 4 bytes at 0x16f8134d8 in main (test/test.c:13)

🔍 Load of 4 bytes at 0x16f8134d8 in main (test/test.c:14)

🔍 Load of 4 bytes at 0x16f8134d8 in main (test/test.c:14)

🔍 Store of 4 bytes at 0x16f81349c in adjust (compiler-inserted:0)

🔍 Load of 4 bytes at 0x16f81349c in adjust (test/test.c:6)

🔍 Store of 4 bytes at 0x16f81349c in adjust (test/test.c:6)

🔍 Load of 4 bytes at 0x16f81349c in adjust (test/test.c:7)




It is expected behavior for some compiler-emitted instructions, especially:

store i32 %0, ptr %2, align 4 → stores the function argument to a local.

store i32 0, ptr %1, align 4 → sets up main's return value placeholder.

These aren’t directly associated with source lines in test.c, so Clang doesn't emit !dbg for them, even with -g.

For such instructions we are going to show compiler-inserted:0