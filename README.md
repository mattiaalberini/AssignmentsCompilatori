PRIMO ASSIGNMENT

Algebraic Identity
opt -load-pass-plugin ../build/libLocalOpts.so -p algebraic-identity AlgebraicIdentity.ll -o AlgebraicIdentity.optimized.bc
llvm-dis AlgebraicIdentity.optimized.bc -o AlgebraicIdentity.optimized.ll

Strength Reduction
opt -load-pass-plugin ../build/libLocalOpts.so -p strength-reduction StrengthReduction.ll -o StrengthReduction.optimized.bc
llvm-dis StrengthReduction.optimized.bc -o StrengthReduction.optimized.ll

Multi-Instruction Optimization
opt -load-pass-plugin ../build/libLocalOpts.so -p multi-instruction MultiInstructionOptimization.ll -o MultiInstructionOptimization.optimized.bc 
llvm-dis MultiInstructionOptimization.optimized.bc -o MultiInstructionOptimization.optimized.ll

clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 fibonacci.C -o fibonacci.ll
opt -p mem2reg Fibonacci.ll -o fibonacci.m2r.bc
llvm-dis fibonacci.m2r.bc -o fibonacci.m2r.ll
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR /Users/eliacapiluppi/Desktop/Assignment/AssignmentCompilatori/PrimoAssignment