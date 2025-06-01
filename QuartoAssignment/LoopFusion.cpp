#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"

using namespace llvm;

namespace {

    struct LoopFusion: PassInfoMixin<LoopFusion> {

        bool isAdjacent(Loop *L0, Loop *L1) {

            if(L0->isGuarded() && L1->isGuarded()) {
                //Tutti e due guarded
                outs() << "Tutti e due guarded\n";

                if(L0->getExitBlock()->getSingleSuccessor() == L1->getLoopGuardBranch()->getParent()) {
                    //Controllo che abbiano lo stesso controllo nella guardia
                    if(auto instrL0 = dyn_cast<Instruction>(L0->getLoopGuardBranch()->getCondition())) {
                        if(auto instrL1 = dyn_cast<Instruction>(L1->getLoopGuardBranch()->getCondition())) {
                            if(instrL0->isIdenticalTo(instrL1)) return true;
                        }
                    }
                }

            } else if (!L0->isGuarded() && !L1->isGuarded()) {
                //Tutti e due non guarded
                outs() << "Tutti e due non guarded\n";

                if(L0->getUniqueExitBlock() == L1->getLoopPreheader()) {
                    return true;
                }

            } else {
                //Uno guarded e uno non guarded. Loop Fusion impossibile
                outs() << "Uno guarded e uno non guarded. Loop Fusion impossibile\n";
            }

            return false; 
        }

        bool sameTripCount(Loop *L0, Loop *L1, ScalarEvolution &SE) {
            auto tripCountL0 = SE.getBackedgeTakenCount(L0);
            auto tripCountL1 = SE.getBackedgeTakenCount(L1);

            if(tripCountL0 == tripCountL1) return true;

            return false;
        }

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
            LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
            ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);

            int i = 0;
            Loop *L0, *L1;
            
            for(auto *L : LI) {
                if(i == 0) L0 = L;
                else L1 = L;

                i++;
            }

            if(isAdjacent(L1, L0)) outs() << "Adiacenti\n";

            if(sameTripCount(L1, L0, SE)) outs() << "Stesso trip count\n";
        
            /*
            SmallVector<Loop *, 8> Worklist = LI.getLoopsInPreorder();

            for(auto L0 = Worklist.begin(); L0 != Worklist.end(); ++L0) {

            }
            */

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
                    if (Name == "loop-fus") {
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