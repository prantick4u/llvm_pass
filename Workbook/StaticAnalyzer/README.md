This pass will detect the overflow during strcpy.


🔧 Compile to LLVM IR

bash

```
clang -S -emit-llvm test.c -o test.ll
```

Resulting LLVM IR (simplified):

```
define void @f(i8* %input) {
entry:
  %buf = alloca [10 x i8], align 1
  %buf_ptr = getelementptr inbounds [10 x i8], [10 x i8]* %buf, i32 0, i32 0
  call i8* @strcpy(i8* %buf_ptr, i8* %input)
  ret void
}
```
🚀 Run the Pass
bash

```
opt -load-pass-plugin ./libStaticOverflowCheck.so \
    -passes=static-overflow-check \
    -disable-output test.ll
```

✅ Expected Output:

Edit
```
🔍 [StaticOverflowCheck] Potential overflow in f:
   Buffer:   %buf = alloca [10 x i8], align 1
   Call:     call i8* @strcpy(i8* %buf_ptr, i8* %input)
```


🔒 Why This Is Useful

✅ This is compile-time vulnerability detection:

Works without executing the program

Finds unsafe patterns before runtime

Can be extended to detect:

Unsafe memcpy

Lack of bounds checks

Dangerous use of return values or tainted inputs




| Issue                 | How to detect in LLVM IR                                  |
| --------------------- | --------------------------------------------------------- |
| **Buffer overflow**   | Find `alloca` arrays & follow to `strcpy`/`memcpy`        |
| **Use-after-free**    | Look for `free()` calls and track pointer usage afterward |
| **Double free**       | Count how many times a pointer is passed to `free()`      |
| **Uninitialized use** | Track variables with no `store` before `load`             |
