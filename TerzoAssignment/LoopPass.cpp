#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/ValueTracking.h"
#include <cmath>

using namespace llvm;

std::set<Instruction*> Invariants;
std::vector<Instruction*> InstToMove;

namespace {
    struct LoopPass: PassInfoMixin<LoopPass> { 

        //Controllo sull'operando
        bool isOperandInvariant(Value *operand, Loop *loop) {
            //Se è costante --> Accettato
            //Se viene da fuori --> Accettato
            if (isa<llvm::Constant>(operand) || isa<llvm::Argument>(operand)) {
                return true;
            }

            if (llvm::Instruction *inst = dyn_cast<llvm::Instruction>(operand)) {
                //Se l'istruzione non fa parte del loop --> Accettato
                //Se l'istruzione è già stata marcata come loop-invariant --> Accettato
                if (!loop->contains(inst->getParent()) || Invariants.count(inst)) {
                    return true;
                }
            }
            return false;
        }


        //Controllo se l'istruzione è loop-invariant
        bool isInstructionLoopInvariant(Instruction *I, Loop *loop) {
            //Controllo che l'istruzione possa essere spostata senza conseguenze
            if (!isSafeToSpeculativelyExecute(I)) {
                return false;
            }

            //Controllo tutti gli operandi dell'istruzione
            for (auto it = I->op_begin(); it != I->op_end(); ++it) {
                if (!isOperandInvariant(*it, loop)) 
                    return false;
            }
            return true;
        }


        //Controllo che non ci sia un uso fuori dal loop
        bool isDead(Instruction *I, Loop *loop) {
            bool isDead = true;

            //Controllo dove viene usata l'istruzione
            for (auto u = I->user_begin(); u != I->user_end() && isDead; ++u) {
                Instruction *instr = dyn_cast<Instruction>(*u);

                if (!loop->contains(instr))
                    //L'istruzione si trova fuori dal loop
                    isDead = false;
            }
            if(isDead) outs() << "Dead\n";
            return isDead;
        }


        //Istruzioni che dominano l'uscita
        void findInstInvariants(Loop *loop, Instruction &I) { 
            if (isInstructionLoopInvariant(&I, loop)) {
                Invariants.insert(&I);
                InstToMove.push_back(&I);
            }
        }


        //Istruzioni che non dominano l'uscita
        void findInstInvariantsDead(Loop *loop, Instruction &I) { 
            if (isInstructionLoopInvariant(&I, loop) && isDead(&I, loop)) {
                Invariants.insert(&I);
                InstToMove.push_back(&I);
            }
        }


        bool runOnLoop(Loop *loop, DominatorTree &DT) {
            //Controllo la presenza del preheader
            BasicBlock* preheader = loop->getLoopPreheader();
            if (!preheader) {
                return false;
            }

            //Vettore con con gli exitBlocks
            SmallVector<BasicBlock*> vec {};
            loop->getExitBlocks(vec);

            auto loopBlocks = loop->getBlocks();

            for (BasicBlock *block : loopBlocks) {

                for (auto &I : *block) {

                    //Controllo, per ogni uscita, che l'istruzione domini l'uscita
                    bool dominateExits = true;
                    for (auto it = vec.begin(); it != vec.end(); ++it) {
                        BasicBlock *exitBlock = *it;                      

                        if (!DT.dominates(&I, exitBlock)) {
                            dominateExits = false;                  
                        }
                    }

                    outs() << I << "\n";  

                    if (dominateExits) {
                        //Domina le uscite
                        findInstInvariants(loop, I);
                        outs() << "Domina le uscite"<< "\n\n";
                    } else {
                        //Non domina le uscite -> deve essere dead
                        findInstInvariantsDead(loop, I);
                        outs() << "Non domina le uscite" << "\n\n";
                    }
                }           
            }

            //Sposto le istruzione nel preheader
            outs() << "Da spostare: \n";
            for (auto &I : InstToMove) {
                outs() << *I <<"\n";
                I->moveBefore(preheader->getTerminator());
            }
            return true;
        }


        PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

            LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
            DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);

            for(auto &L : LI) {
                if (!runOnLoop(L, DT)) {
                    outs() << "Errore!\n";
                    return PreservedAnalyses::all();
                }
            }           
            
            return PreservedAnalyses::all();
        }

        static bool isRequired() { return true; }
    };
}

// Pass Plugin Registration
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
