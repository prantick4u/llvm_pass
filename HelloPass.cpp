#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {

struct HelloPass : PassInfoMixin<HelloPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    errs() << "[HelloPass] Function: " << F.getName() << "\n";
    return PreservedAnalyses::all();
  }
};

} // namespace

// New plugin registration
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "hellopass", LLVM_VERSION_STRING,
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
    }};
}