#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {
struct MemoryInstrumentationPass : PassInfoMixin<MemoryInstrumentationPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    LLVMContext &Ctx = F.getContext();
    Module *M = F.getParent();
    IRBuilder<> builder(Ctx);

    // Use PointerType::getUnqual(...) to get i8*
    Type *Int8Ty = Type::getInt8Ty(Ctx);
    Type *Int32Ty = Type::getInt32Ty(Ctx);
    Type *VoidTy = Type::getVoidTy(Ctx);
    Type *Int8PtrTy = PointerType::getUnqual(Int8Ty);

    // Declare the logger function: void __log_access(void*, char, int, const char*, const char*, int)
    FunctionCallee logFn = M->getOrInsertFunction("__log_access",
      FunctionType::get(VoidTy, { Int8PtrTy, Int8Ty, Int32Ty, Int8PtrTy, Int8PtrTy, Int32Ty }, false));

    for (auto &BB : F) {
      for (auto &I : BB) {
        Value *addr = nullptr;
        bool isStore = false;
        unsigned size = 0;

        if (auto *load = dyn_cast<LoadInst>(&I)) {
          addr = load->getPointerOperand();
          isStore = false;
          size = load->getType()->getPrimitiveSizeInBits() / 8;
        } else if (auto *store = dyn_cast<StoreInst>(&I)) {
          addr = store->getPointerOperand();
          isStore = true;
          size = store->getValueOperand()->getType()->getPrimitiveSizeInBits() / 8;
        }

        if (addr) {
          builder.SetInsertPoint(&I);

          std::string funcName = F.getName().str();
          std::string fileName = "compiler-inserted";
          unsigned line = 0;
          if (DILocation *loc = I.getDebugLoc()) {
              fileName = loc->getFilename().str();
              if (fileName.empty() && loc->getScope())
                  if (auto *scope = dyn_cast<DIScope>(loc->getScope()))
                      fileName = scope->getFilename().str();
              line = loc->getLine();
          }
          Value *args[] = {
            builder.CreatePointerCast(addr, Int8PtrTy),
            builder.getInt8(isStore ? 1 : 0),
            builder.getInt32(size),
            builder.CreateGlobalString(funcName),
            builder.CreateGlobalString(fileName),
            builder.getInt32(line)
          };

          builder.CreateCall(logFn, args);
        }
      }
    }

    return PreservedAnalyses::none();
  }
};
} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "MemoryInstrumentation", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, ModulePassManager &MPM, ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "mem-instrument") {
            MPM.addPass(createModuleToFunctionPassAdaptor(MemoryInstrumentationPass()));
            return true;
          }
          return false;
        });
    }
  };
}
