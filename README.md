# llvm_pass

An LLVM pass is a modular unit of code that analyzes or transforms LLVM intermediate representation (IR) during compilation.
It enables optimizations, instrumentation, or code transformations, forming the core of LLVM’s flexible compiler pipeline.

## 🔧 Real-Life Examples of LLVM Passes

LLVM passes are essential for optimizing and transforming code during compilation. Here are some practical examples used in real-world systems:

### 🚀 Common Optimization Passes

- **Dead Code Elimination (DCE):**  
  Removes unused instructions or functions to reduce binary size and improve execution speed.

- **Loop Unrolling:**  
  Expands loops into multiple repeated statements to reduce branching and enable vectorization.

- **Function Inlining:**  
  Replaces function calls with the function's body to eliminate call overhead and expose further optimization opportunities.

### 🧪 Analysis and Debugging Passes

- **AddressSanitizer (ASan):**  
  Instruments code to detect memory errors such as buffer overflows, use-after-free, and stack corruption at runtime.

- **GCOV Profiling Pass:**  
  Injects profiling counters into the IR to measure code coverage during test runs.

### 🛡️ Custom & Security-Oriented Passes

- **Security Hardening Pass:**  
  Inserts stack canaries or bounds checks to mitigate buffer overflow and other common attack vectors.

- **Custom Logging/Tracing Pass:**  
  Adds logging statements to monitor function entry/exit or variable usage for profiling and debugging.

---

These passes illustrate the flexibility and power of LLVM's modular pass infrastructure, which enables fine-grained control over code transformation and analysis workflows.




## 🔧 Build LLVM project only to run Passes

### Steps:
- Install required prerequisites
```
sudo apt update
sudo apt install cmake ninja-build git clang libedit-dev python3 zlib1g-dev libxml2-dev

```
- Checkout to stable version

```
git checkout llvmorg-14.0.0
```

- Run `cmake` command

```
mkdir build && cd build

cmake -G Ninja ../llvm \
  -DLLVM_ENABLE_PROJECTS="llvm" \
  -DLLVM_TARGETS_TO_BUILD="X86" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_INCLUDE_TESTS=OFF \
  -DLLVM_INCLUDE_EXAMPLES=OFF \
  -DLLVM_INCLUDE_DOCS=OFF

```

- Build 
```
ninja opt llvm-config -j 2
```

