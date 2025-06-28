<!-- This was cmpiling with O0. 
LLVM adds the optnone attribute to functions when compiled with -O0.
And when a function has optnone, LLVM's pass manager skips all 
optimization passes, including custom ones — even if they don't modify IR. -->
clang -S -emit-llvm vuln.c -o vuln.llcd 

<!-- Regenerate IR with optimizations enabled. That way, optnone won’t be inserted -->
clang -S -emit-llvm -O1 test.cpp -o test.ll

<!-- Run the pass afer loading HelloPass on .ll -->
opt -load-pass-plugin ./build/HelloPass.so -passes=hellopass < ./test.ll -disable-output 2>&1

<!-- debug pass manager shows details of the steps -->
opt -debug-pass-manager -load-pass-plugin ./build/HelloPass.so -passes=hellopass < test.ll -disable-output 2>&1


What we achieved:

✅ This HelloPass plugin is registered via llvmGetPassPluginInfo()

✅ It’s recognized by the new pass manager via registerPipelineParsingCallback()

✅ It visits each function in the IR file (test.ll) which is geneated from `clang -S -emit-llvm ` on .cpp file.

✅ Emits the expected side effect (outs() output)



Why This Is Important
This is the foundation for:

🚀 Writing transformation passes (modifying IR)

🔍 Writing analysis passes (gathering information)

🧪 Building custom sanitizers, instrumentation, or security checkers

⚔️ Exploring miscompilation, attack surfaces, or mitigations


