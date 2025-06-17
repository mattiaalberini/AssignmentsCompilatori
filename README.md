## PRIMO ASSIGNMENT

**Algebraic Identity:** \
opt -load-pass-plugin ../build/libLocalOpts.so -p algebraic-identity AlgebraicIdentity.ll -o AlgebraicIdentity.optimized.bc \
llvm-dis AlgebraicIdentity.optimized.bc -o AlgebraicIdentity.optimized.ll

**Strength Reduction:** \
opt -load-pass-plugin ../build/libLocalOpts.so -p strength-reduction StrengthReduction.ll -o StrengthReduction.optimized.bc \
llvm-dis StrengthReduction.optimized.bc -o StrengthReduction.optimized.ll

**Multi-Instruction Optimization:** \
opt -load-pass-plugin ../build/libLocalOpts.so -p multi-instruction MultiInstructionOptimization.ll -o MultiInstructionOptimization.optimized.bc \
llvm-dis MultiInstructionOptimization.optimized.bc -o MultiInstructionOptimization.optimized.ll

## TERZO ASSIGNMENT

opt -load-pass-plugin ../build/libLoopPass.dylib -p loop-pass LoopCodeM.m2r.ll -o LoopPass.optimized.bc
llvm-dis LoopPass.optimized.bc -o LoopPass.optimized.ll

## QUARTO ASSIGNMENT

opt -load-pass-plugin ../build/libLoopFusion.so -p loop-fus Loops.ll -o Loops.optimized.bc
llvm-dis Loops.optimized.bc -o Loops.optimized.ll

## VARIE

clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 Loops.c -o Loops.app.ll
opt -p mem2reg Loops.app.ll -o Loops.app.bc
llvm-dis Loops.app.bc -o Loops.ll