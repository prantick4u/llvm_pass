#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

struct MemoryInstrumentationPass : PassInfoMixin<MemoryInstrumentationPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    LLVMContext &Ctx = F.getContext();
    Module *M = F.getParent();

    // Declare or get reference to the logging function: void __log_access(i8*)
    FunctionCallee LogFunc = M->getOrInsertFunction(
        "__log_access", FunctionType::get(Type::getVoidTy(Ctx),
                                          {PointerType::get(Type::getInt8Ty(Ctx), 0)}, false));

    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *LI = dyn_cast<LoadInst>(&I)) {
          IRBuilder<> Builder(LI);
          Value *Ptr = LI->getPointerOperand();
          Value *Cast = Builder.CreatePointerCast(Ptr, PointerType::get(Type::getInt8Ty(Ctx), 0));
          Builder.CreateCall(LogFunc, {Cast});
        } else if (auto *SI = dyn_cast<StoreInst>(&I)) {
          IRBuilder<> Builder(SI);
          Value *Ptr = SI->getPointerOperand();
          Value *Cast = Builder.CreatePointerCast(Ptr, PointerType::get(Type::getInt8Ty(Ctx), 0));
          Builder.CreateCall(LogFunc, {Cast});
        }
      }
    }

    return PreservedAnalyses::none();
  }
};

// Register the pass plugin
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "MemoryInstrumentationPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "mem-instrument") {
            FunctionPassManager FPM;
            FPM.addPass(MemoryInstrumentationPass());
            MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
            return true;
          }
          return false;
        });
    }
  };
}
