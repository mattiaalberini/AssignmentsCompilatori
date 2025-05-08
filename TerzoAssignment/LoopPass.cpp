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
        
        bool runOnBasicBlock(BasicBlock &B) {
            return true;
        }

        bool isOperandInvariant(Value *operand, Loop &loop) {
            if (isa<llvm::Constant>(operand) || isa<llvm::Argument>(operand)) {
                return true;
            }

            if (llvm::Instruction *inst = dyn_cast<llvm::Instruction>(operand)) {
                if (!loop.contains(inst->getParent()) || Invariants.count(inst)) {
                    return true;
                }
            }
            return false;
        }

        bool isInstructionLoopInvariant(Instruction *I, Loop &loop) {
            if (!isSafeToSpeculativelyExecute(I)) {
                return false;
            }
            for (auto it = I->op_begin(); it != I->op_end(); ++it) {
                if (!isOperandInvariant(*it, loop)) 
                    return false;
            }
            return true;
        }

        bool isDead(Instruction *I, Loop &loop) {
            bool isDead = true;
            for (auto u = I->use_begin(); u != I->use_end() && isDead; ++u) {
                Instruction *instr = dyn_cast<Instruction>(&*u);
                if (!loop.contains(instr))
                    isDead = false;
            }
            return isDead;
        }

        void findInstInvariants(Loop &loop, BasicBlock &block) {
            for (auto &I : block) {
                if (isInstructionLoopInvariant(&I, loop)) {
                    Invariants.insert(&I);
                    InstToMove.push_back(&I);
                }
            }
        }

        void findInstInvariantsDead(Loop &loop, BasicBlock &block) {
            for (auto &I : block) {
                if (isInstructionLoopInvariant(&I, loop) && isDead(&I, loop)) {
                    Invariants.insert(&I);
                    InstToMove.push_back(&I);
                }
            }
        }

        bool runOnLoop(Loop &loop, LoopAnalysisManager &LAM, LoopStandardAnalysisResults &LAR, LPMUpdater &LU) {
            BasicBlock* preheader = loop.getLoopPreheader();
            if (!preheader) {
                return false;
            }

            SmallVector<BasicBlock*> vec {};
            loop.getExitBlocks(vec);
            llvm::DominatorTree &DT = LAR.DT;
            auto loopBlocks = loop.getBlocks();
            for (auto &block : loopBlocks) {
                bool dominateExits = true;
                for (auto it = vec.begin(); it != vec.end(); ++it) {
                    BasicBlock *exitBlock = *it;
                    if (!DT.dominates(block, exitBlock))
                        dominateExits = false;
                }

                if (dominateExits)
                    findInstInvariants(loop, *block);
                else
                    findInstInvariantsDead(loop, *block);
            }

            for (auto &I : InstToMove) {
                I->moveBefore(preheader->getTerminator());
            }
            return true;
        }

        PreservedAnalyses run(Loop &L, LoopAnalysisManager &LAM, LoopStandardAnalysisResults &LAR, LPMUpdater &LU) {
            if (!runOnLoop(L, LAM, LAR, LU)) {
                return PreservedAnalyses::none();
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
            [](StringRef Name, LoopPassManager &LPM,
               ArrayRef<PassBuilder::PipelineElement>) {
              if (Name == "loop-pass") {
                LPM.addPass(LoopPass());
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
