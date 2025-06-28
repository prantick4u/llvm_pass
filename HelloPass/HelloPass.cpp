// HelloPass.cpp - Modern LLVM Pass using New Pass Manager
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

// Define the pass
struct HelloPass : public PassInfoMixin<HelloPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    errs() << "Hello from: " << F.getName() << "\n";
    errs().flush();
    return PreservedAnalyses::all();
  }
};

// Plugin registration
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "HelloPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "hellopass") {
            FPM.addPass(HelloPass());
            return true;
          }
          return false;
        });
    }
  };
}
