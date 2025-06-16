#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/DependenceAnalysis.h"

using namespace llvm;

namespace {

    struct LoopFusion: PassInfoMixin<LoopFusion> {

        //Controllo se sono adiacenti
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

        //Controllo se iterano lo stesso numero di volte
        bool sameTripCount(Loop *L0, Loop *L1, ScalarEvolution &SE) {
            auto tripCountL0 = SE.getBackedgeTakenCount(L0);
            auto tripCountL1 = SE.getBackedgeTakenCount(L1);

            //SE.getBackedgeTakenCount(L1) ritorna un SCEVCouldNotCompute se non è possibile prevedere il numero di backedge-taken
            if(auto *testErrL0 = dyn_cast<SCEVCouldNotCompute>(tripCountL0)) return false;
            if(auto *testErrL1 = dyn_cast<SCEVCouldNotCompute>(tripCountL1)) return false;

            if(tripCountL0 == tripCountL1) return true;

            return false;
        }

        //Controllo se sono control flow equivalent
        bool isControlFlowEquivalence(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT){
            if(L0->isGuarded() && L1->isGuarded()) {
                //Tutti e due guarded
                if(DT.dominates(L0->getExitBlock()->getSingleSuccessor(), L1->getLoopGuardBranch()->getParent())) {
                    outs() << "L0 domina L1\n";
                    if(PDT.dominates(L1->getLoopGuardBranch()->getParent(), L0->getExitBlock()->getSingleSuccessor())) {
                        outs() << "L1 postdomina L0\n";
                        return true;
                    }
                }

            } else {
                //Tutti e due non guarded
                if(DT.dominates(L0->getUniqueExitBlock(), L1->getLoopPreheader())) {
                    outs() << "L0 domina L1\n";
                    if(PDT.dominates(L1->getLoopPreheader(), L0->getUniqueExitBlock())) {
                        outs() << "L1 postdomina L0\n";
                        return true;
                    }
                }
            } 

            return false;
        }

        //Controllo se ci sono dipendenze negative
        bool hasNotNegativeDependence(Loop *L0, Loop *L1, DependenceInfo &DI) {
            
            for(auto &BBL0 : L0->getBlocks()){
                for(Instruction &IL0 : *BBL0) {
                    if(auto *storeInstL0 = dyn_cast<StoreInst>(&IL0)) {
                        //Nel primo loop c'è una store
                        auto *StorePtr = storeInstL0->getPointerOperand();

                        for(auto &BBL1 : L1->getBlocks()){
                            for(Instruction &IL1 : *BBL1) {
                                if(auto *loadInstL1 = dyn_cast<LoadInst>(&IL1)) {
                                    //Nel secondo loop c'è una load
                                    auto *LoadPtr = loadInstL1->getPointerOperand();
                                    
                                    if(auto Dep = DI.depends(&IL0, &IL1, true)) {

                                        //Istruzioni con potenziale dipendenza
                                        if(Dep) {
                                            auto *GEPStore = dyn_cast<GetElementPtrInst>(StorePtr);
                                            auto *GEPLoad = dyn_cast<GetElementPtrInst>(LoadPtr);

                                            //Controllo se è lo stesso array
                                            if(GEPStore->getPointerOperand() == GEPLoad->getPointerOperand()) {
                                                auto *IndexStore = GEPStore->getOperand(1);
                                                auto *IndexLoad = GEPLoad->getOperand(1);
                                                
                                                //Controllo l'offset
                                                if(auto *IndexStoreInst = dyn_cast<Instruction>(IndexStore)) {
                                                    if(auto *IndexLoadInst = dyn_cast<Instruction>(IndexLoad)) {

                                                        outs() << IL0 << "\n";
                                                        outs() << IL1 << "\n";

                                                        outs() << *IndexStoreInst->getOperand(0) << "\n";
                                                        outs() << *IndexLoadInst->getOperand(0) << "\n";

                                                        //Se sono tutte due PHI Instruction l'accesso all'array avviene tramite induction variable
                                                        if(PHINode *PHIStore = dyn_cast<PHINode>(IndexStoreInst->getOperand(0))) {
                                                            if(PHINode *PHIStore = dyn_cast<PHINode>(IndexLoadInst->getOperand(0))) {
                                                                outs() << "Nessuna dipendenze negativa\n";
                                                            } else {
                                                                outs() << "Dipendenze negativa\n";
                                                                return false;
                                                            }
                                                        } else {
                                                            outs() << "Dipendenze negativa\n";
                                                            return false;
                                                        }
                                                    }
                                                }
                                                
                                            }

                                        } else {
                                            //Non hanno dipendenza
                                            return true;
                                        }                                                                     
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }
        
        //Unisco i loop
        bool loopFus(Loop *L0, Loop *L1) {
            auto *IVL0 = L0->getCanonicalInductionVariable();
            auto *IVL1 = L1->getCanonicalInductionVariable();

            //Modifico gli usi della induction variable
            IVL1->replaceAllUsesWith(IVL0);

            //Modifiche CFG

            auto HeaderL0 = L0->getHeader();
            auto HeaderL1 = L1->getHeader();

            auto ExitL1 = L1->getUniqueExitBlock();

            auto LatchL0 = L0->getLoopLatch();
            auto LatchL1 = L1->getLoopLatch();


            if(L0->isGuarded() && L1->isGuarded()) {
                //Guarded
                
                auto GuardL0 = L0->getLoopGuardBranch()->getParent();
                auto GuardL1 = L1->getLoopGuardBranch()->getParent();

                BranchInst::Create(L0->getLoopPreheader(), ExitL1, GuardL0->back().getOperand(0), GuardL0->getTerminator());
                GuardL0->getTerminator()->eraseFromParent();

                BranchInst::Create(L0->getBlocks().front(), ExitL1, LatchL0->back().getOperand(0), LatchL0->getTerminator());
                LatchL0->getTerminator()->eraseFromParent();

                //Body L0 -> Body L1
                L0->getBlocks().drop_back(1).back()->getTerminator()->setSuccessor(0, L1->getBlocks().front());

                //Body L1 -> Latch L0
                L1->getBlocks().drop_back(1).back()->getTerminator()->setSuccessor(0, LatchL0);

                HeaderL1->front().eraseFromParent();

            } else {
                //Non guarded

                //Non considero nel body l'header e il latch
                auto BodyL0 = L0->getBlocks().drop_front(1).drop_back(1);
                auto BodyL1 = L1->getBlocks().drop_front(1).drop_back(1);

                //Body L0 -> Body L1
                BodyL0.back()->getTerminator()->setSuccessor(0, BodyL1.front());

                //Body L1 -> Latch L0
                BodyL1.back()->getTerminator()->setSuccessor(0, LatchL0);

                //Header L1 -> Latch L1
                BranchInst::Create(LatchL1, HeaderL1->getTerminator());
                HeaderL1->getTerminator()->eraseFromParent();
                
                //Header L0 -> Exit L1
                BranchInst::Create(BodyL0.front(), ExitL1, HeaderL0->back().getOperand(0), HeaderL0->getTerminator());
                HeaderL0->getTerminator()->eraseFromParent();
            }

            return true;
        }


        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
            LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
            ScalarEvolution &SE = AM.getResult<ScalarEvolutionAnalysis>(F);
            DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
            PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
            DependenceInfo &DI = AM.getResult<DependenceAnalysis>(F);

            
            SmallVector<Loop *, 8> Worklist;
            
            Loop *L0, *L1;
            
            for(Loop *TopLevelLoop : LI) {
                for(Loop *L : depth_first(TopLevelLoop)) {
                    if(L->isInnermost()) {
                        Worklist.push_back(L);
                    }
                }
            }         

            if(Worklist.size() < 2) {
                outs() << "Non ci sono abbastanza loop\n";
                return PreservedAnalyses::all();
            }

            for(int i = 0; i < Worklist.size(); ++i) {
                for(int j = i + 1; j < Worklist.size(); ++j) {
                    L0 = Worklist[i];
                    L1 = Worklist[j];

                    if(isAdjacent(L1, L0)) {
                        outs() << "Adiacenti\n";

                        if(sameTripCount(L1, L0, SE)) {
                            outs() << "Stesso trip count\n";

                            if(isControlFlowEquivalence(L1, L0, DT, PDT)) {
                                outs() << "Control flow equivalent\n";
                                
                                if(hasNotNegativeDependence(L1, L0, DI)) {
                                    outs() << "Nessuna dipendenza a distanza negativa\n";

                                    if(loopFus(L1, L0)) outs() << "Loop fusion avvenuta correttamente\n";
                                    else outs() << "Errore durante loop fusion\n";
                                }
                            }
                        }
                    }
                }
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