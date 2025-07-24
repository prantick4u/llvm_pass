1. ✅ Run your LLVM Pass

    clang -S -emit-llvm test.c -o test.ll -Xclang -disable-O0-optnone -O0

    Note: We need to remove the optnone from the test.ll file using the flag `-Xclang -disable-O0-optnone`, otherwise the functions will be skipped.

    opt -load-pass-plugin ./build/MemoryInstrumentation.dylib -passes=mem-instrument < test.ll > instrumented.ll

    Now instrumented.ll includes `call void @__log_access(i8*)` before every load/store.

2. 🏗 Compile instrumented.ll to object code
    
    llc instrumented.ll -filetype=obj -o instrumented.o
    
    This creates instrumented.o, a compiled object file.

3. 🧱 Compile the logger function
    
    clang -c logger/logger.c -o logger.o   

    This creates logger.o with your __log_access implementation.

4. 🔗 Link both to create final executable
    
    clang instrumented.o logger.o -o final_program

5. 🏃 Run it
    
    ./final_program

The output would be like:

🔍 Access at address: 0x16af8f4ac
🔍 Access at address: 0x16af8f4a8
🔍 Access at address: 0x16af8f4a8
🔍 Access at address: 0x16af8f4a8
🔍 Access at address: 0x16af8f47c
🔍 Access at address: 0x16af8f47c
🔍 Access at address: 0x16af8f47c
🔍 Access at address: 0x16af8f47c