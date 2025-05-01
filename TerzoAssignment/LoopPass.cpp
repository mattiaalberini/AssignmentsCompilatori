#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {

  struct LoopPass: PassInfoMixin<LoopPass> {
    
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      
      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);

      if(LI.empty()) {
        outs() << "NO LOOP:\n";
        
      } else {
        outs() << "LOOP:\n";

        for(auto &L : LI) {

          if(L->isLoopSimplifyForm()) outs() << "Loop in forma normale \n";

          BasicBlock *BH = L->getHeader();
          Function *F = BH->getParent();
          outs() << F->getName();
          //call F->viewCFGOnly();
          
          for(auto BI = L->block_begin(); BI != L->block_end(); ++BI) {
            BasicBlock *BB = *BI;

            BB->print(outs());
  
            if(LI.isLoopHeader(BB)) {
              outs() << "Header \n";
            } else {
              outs() << "Non header \n";
            }
          }
        }
      }

      return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
  };
}

llvm::PassPluginLibraryInfo getLoopPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "loop-pass") {
                    FPM.addPass(LoopPass());
                    return true;
                  }
                  return false;
                });
          }};
}


extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopPassPluginInfo();
}
