#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/IRBuilder.h" // 🔑 needed for IRBuilder

using namespace llvm;

struct ReplaceMultWithAddPass : PassInfoMixin<ReplaceMultWithAddPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    for (auto &BB : F) {
      for (auto it = BB.begin(); it != BB.end(); ) {
        if (auto *op = dyn_cast<BinaryOperator>(&*it)) {
          if (op->getOpcode() == Instruction::Mul) {
            IRBuilder<> builder(op);
            auto *Add = builder.CreateAdd(op->getOperand(0), op->getOperand(1));
            op->replaceAllUsesWith(Add);
            it = std::next(it);        // move before erase!
            op->eraseFromParent();
            continue;
          }
        }
        ++it;
      }
    }
    return PreservedAnalyses::none();
  }
};

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "ReplaceMultWithAdd", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
          ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "replace-mult-with-add") {
            FunctionPassManager FPM;
            FPM.addPass(ReplaceMultWithAddPass());
            MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
            return true;
          }
          return false;
        }
      );
    }
  };
}
