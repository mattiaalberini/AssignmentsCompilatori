#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <cmath>
//commento di prova
using namespace llvm;

namespace {

    struct AlgebraicIdentity: PassInfoMixin<AlgebraicIdentity> {
        
        bool runOnBasicBlock(BasicBlock &B) {
            
            for (auto &I : B) {
                if(dyn_cast<BinaryOperator>(&I)) {
                    //Moltiplicazione
                    if(I.getOpcode() == Instruction::Mul) {
                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(0))) {
                            if(C->getValue() == 1) {
                                I.replaceAllUsesWith(I.getOperand(1));
                            }
                        }
                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(1))) {
                            if(C->getValue() == 1) {
                                I.replaceAllUsesWith(I.getOperand(0));
                            }
                        }
                    }

                    //Addizione
                    if(I.getOpcode() == Instruction::Add) {
                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(0))) {
                            if(C->getValue() == 0) {
                                I.replaceAllUsesWith(I.getOperand(1));
                            }
                        }
                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(1))) {
                            if(C->getValue() == 0) {
                                I.replaceAllUsesWith(I.getOperand(0));
                            }
                        }
                    }
                }
            }
    
            return true;
        } 
    
        bool runOnFunction(Function &F) {
            bool Transformed = false;
    
            for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
                if(runOnBasicBlock(*Iter)) {
                    Transformed = true;
                }
            }
            
            return Transformed;
        }

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
            bool Transformed = runOnFunction(F);
    
            return PreservedAnalyses::all();
        }
    
    
        static bool isRequired() { return true; }
    };

    struct StrengthReduction: PassInfoMixin<StrengthReduction> {
        
        bool runOnBasicBlock(BasicBlock &B) {
            
            for (auto &I : B) {
                if(dyn_cast<BinaryOperator>(&I)) {
                    if(I.getOpcode() == Instruction::Mul) {
                        
                        outs() << "MOLTIPLICAZIONE\n";
                        
                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(0))) {
                            outs() << "PRIMO OPERANDO\n";
                            
                            if(C->getValue().isPowerOf2()) {
                                outs() << "MULTIPLO DI 2\n";
                            
                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(1), ConstantInt::get(C->getType(), C->getValue().exactLogBase2()));
                                NewInst->insertAfter(&I);
                                I.replaceAllUsesWith(NewInst);
                            }
                        }

                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(1))) {
                            outs() << "SECONDO OPERANDO\n";
                            
                            if(C->getValue().isPowerOf2()) {
                                outs() << "MULTIPLO DI 2\n";

                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(0), ConstantInt::get(C->getType(), C->getValue().exactLogBase2()));
                                NewInst->insertAfter(&I);
                                I.replaceAllUsesWith(NewInst);
                            }
                        }

                    }
                }
            }
    
            return true;
        } 
    
        bool runOnFunction(Function &F) {
            bool Transformed = false;
    
            for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
                if(runOnBasicBlock(*Iter)) {
                    Transformed = true;
                }
            }
            
            return Transformed;
        }

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
            bool Transformed = runOnFunction(F);
    
            return PreservedAnalyses::all();
        }
    
    
        static bool isRequired() { return true; }
    };
} 

llvm::PassPluginLibraryInfo getLocalOptsPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "LocalOpts", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                    ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "algebraic-identity") {
                        FPM.addPass(AlgebraicIdentity());
                        return true;
                    }
                    if (Name == "strength-reduction") {
                        FPM.addPass(StrengthReduction());
                        return true;
                    }
                
                    return false;
                });
            }};
}
  

  extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
  llvmGetPassPluginInfo() {
    return getLocalOptsPluginInfo();
  }