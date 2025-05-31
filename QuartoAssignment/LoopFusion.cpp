#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

namespace {

    struct LoopFusion: PassInfoMixin<LoopFusion> {

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
            LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
            SmallVector<Loop *, 8> Worklist;

            outs() << "Test";
            
            for(auto *L : LI) {
                outs() << L;
            }
    
            return PreservedAnalyses::all();
        }
    
        static bool isRequired() { return true; }
    };

} 

llvm::PassPluginLibraryInfo getLoopFusionPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "LoopFusion", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                    ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "loop-fusion") {
                        FPM.addPass(LoopFusion());
                        return true;
                    }
                
                    return false;
                });
            }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getLoopFusionPluginInfo();
}