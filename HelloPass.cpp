// HelloPass.cpp
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct HelloPass : public FunctionPass {
    static char ID;
    HelloPass() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      errs() << "Found function: " << F.getName() << "\n";
      return false; // No modification
    }
  };
}

char HelloPass::ID = 0;
static RegisterPass<HelloPass> X("hellopass", "Hello LLVM Pass");
