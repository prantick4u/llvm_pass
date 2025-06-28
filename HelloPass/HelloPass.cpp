// HelloPass.cpp - LLVM Pass with module-to-function adaptor (LLVM 14+)
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

// Define the function-level pass
struct HelloPass : PassInfoMixin<HelloPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    errs()<< "Hello from: " << F.getName() << "\n";
    errs().flush();
    return PreservedAnalyses::all();
  }
};

// Register plugin for opt
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "HelloPass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "hellopass") {
            outs()<<"HelloPass : added"<<"\n";
            FunctionPassManager FPM;
            // Manage passes that run on each Function
            FPM.addPass(HelloPass());
            // Manage passes that run on each Module.
            // LLVM expects you to only add module-level passes to the MPM.
            // So if you have function-level logic, you must wrap it.
            // createModuleToFunctionPassAdaptor ensures to run this function-level pipeline over every function in the module.
            MPM.addPass(createModuleToFunctionPassAdaptor(std::move(FPM)));
            return true;
          }
          return false;
        });
    }
  };
}
