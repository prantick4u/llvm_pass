#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

struct StaticOverflowCheckPass : PassInfoMixin<StaticOverflowCheckPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *alloca = dyn_cast<AllocaInst>(&I)) {
          if (alloca->getAllocatedType()->isArrayTy()) {
            for (User *U : alloca->users()) {
              if (auto *call = dyn_cast<CallInst>(U)) {
                if (Function *called = call->getCalledFunction()) {
                  StringRef name = called->getName();
                  if (name.contains("strcpy") || name.contains("__strcpy_chk")) {
                    errs() << "🔍 [StaticOverflowCheck] Potential overflow in [direct] " << F.getName() << ":\n";
                    errs() << "   Buffer: " << *alloca << "\n";
                    errs() << "   Call:   " << *call << "\n";
                  }
                }
              } else if (Instruction *I = dyn_cast<Instruction>(U)) {
                for (User *subU : I->users()) {
                  if (auto *call = dyn_cast<CallInst>(subU)) {
                    if (Function *called = call->getCalledFunction()) {
                      StringRef name = called->getName();
                      if (name.contains("strcpy") || name.contains("__strcpy_chk")) {
                        errs() << "🔍 [StaticOverflowCheck] Potential overflow in [indirect] " << F.getName() << ":\n";
                        errs() << "   Buffer: " << *alloca << "\n";
                        errs() << "   Call:   " << *call << "\n";
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    return PreservedAnalyses::all();
  }
};


extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "StaticOverflowCheck", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
          ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "static-overflow-check") {
            FunctionPassManager FPM;
            FPM.addPass(StaticOverflowCheckPass());
            MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
            return true;
          }
          return false;
        }
      );
    }
  };
}