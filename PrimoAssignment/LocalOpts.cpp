#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <cmath>

using namespace llvm;

namespace {

    //opt -load-pass-plugin /home/mattia/workspace/Laboratori_Compilatori/AssignmentCompilatori/PrimoAssignment/build/libLocalOpts.so -p algebraic-identity TestAlgebraicIdentity.ll -o TestAlgebraicIdentity.optimized.bc

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
                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(0))) {
                            outs() << "PRIMO OPERANDO\n";
                            if(C->getValue().isPowerOf2()) {
                                outs() << "MULTIPLO DI 2\n";
                            
                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(1), ConstantInt::get(C->getType(), C->getValue().exactLogBase2()));
                                NewInst->insertAfter(&I);
                                I.replaceAllUsesWith(NewInst);
                                continue;
                            }
                            if((C->getValue()+1).isPowerOf2()) {
                                outs() << "sotto a 2\n";
                            
                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(1), ConstantInt::get(C->getType(), (C->getValue()+1).exactLogBase2()));
                                NewInst->insertAfter(&I);
                                //sottriamo x
                                Instruction *NewInst1 = BinaryOperator::Create(Instruction::Sub, NewInst, I.getOperand(1));
                                NewInst1->insertAfter(NewInst);
                                I.replaceAllUsesWith(NewInst1);
                                continue;
                            }
                            if((C->getValue()-1).isPowerOf2()) {
                                outs() << "sopra a 2\n";
                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(1), ConstantInt::get(C->getType(), (C->getValue()-1).exactLogBase2()));
                                NewInst->insertAfter(&I);
                                //sommiamo x
                                Instruction *NewInst1 = BinaryOperator::Create(Instruction::Add, NewInst, I.getOperand(1));
                                NewInst1->insertAfter(NewInst);
                                I.replaceAllUsesWith(NewInst1);
                                continue;
                            }
                        }

                        if(ConstantInt *C = dyn_cast<ConstantInt>(I.getOperand(1))) {
                            outs() << "SECONDO OPERANDO\n";
                            
                            if(C->getValue().isPowerOf2()) {
                                outs() << "MULTIPLO DI 2\n";

                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(0), ConstantInt::get(C->getType(), C->getValue().exactLogBase2()));
                                NewInst->insertAfter(&I);
                                I.replaceAllUsesWith(NewInst);
                                continue;
                            }
                            if((C->getValue()+1).isPowerOf2()) {
                                outs() << "sotto a 2\n";
                            
                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(0), ConstantInt::get(C->getType(), (C->getValue()+1).exactLogBase2()));
                                NewInst->insertAfter(&I);
                                //sottriamo x
                                Instruction *NewInst1 = BinaryOperator::Create(Instruction::Sub, NewInst, I.getOperand(0));
                                NewInst1->insertAfter(NewInst);
                                I.replaceAllUsesWith(NewInst1);
                                continue;
                            }
                            if((C->getValue()-1).isPowerOf2()) {
                                outs() << "sopra a 2\n";
                                Instruction *NewInst = BinaryOperator::Create(Instruction::Shl, I.getOperand(0), ConstantInt::get(C->getType(), (C->getValue()-1).exactLogBase2()));
                                NewInst->insertAfter(&I);
                                //sommiamo x
                                Instruction *NewInst1 = BinaryOperator::Create(Instruction::Add, NewInst, I.getOperand(0));
                                NewInst1->insertAfter(NewInst);
                                I.replaceAllUsesWith(NewInst1);
                                continue;
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