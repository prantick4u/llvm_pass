#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

struct StaticOverflowCheckPass : PassInfoMixin<StaticOverflowCheckPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *alloca = dyn_cast<AllocaInst>(&I)) {
          if (alloca->getAllocatedType()->isArrayTy()) {
            // Now check for uses of this alloca in calls
            for (User *U : alloca->users()) {
              if (auto *gep = dyn_cast<GetElementPtrInst>(U)) {
                for (User *gepUser : gep->users()) {
                  if (auto *call = dyn_cast<CallInst>(gepUser)) {
                    if (Function *called = call->getCalledFunction()) {
                      if (called->getName() == "strcpy") {
                        Value *src = call->getArgOperand(1);
                        if (isa<Argument>(src)) {
                          errs() << "🔍 [StaticOverflowCheck] Potential overflow in " << F.getName() << ":\n";
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
    }
    return PreservedAnalyses::all();
  }
};


extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "StaticOverflowCheck", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
          ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "static-overflow-check") {
            FPM.addPass(StaticOverflowCheckPass());
            return true;
          }
          return false;
        }
      );
    }
  };
}