clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 fibonacci.C -o fibonacci.ll
opt -p mem2reg Fibonacci.ll -o fibonacci.m2r.bc
llvm-dis fibonacci.m2r.bc -o fibonacci.m2r.ll
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR /Users/eliacapiluppi/Desktop/Assignment/AssignmentCompilatori/PrimoAssignment
opt -load-pass-plugin /Users/eliacapiluppi/Desktop/Assignment/AssignmentCompilatori/PrimoAssignment/build/libLocalOpts.dylib -p strength-reduction StrengthReduction.ll -o StrengthReduction.optimized.bc
llvm-dis StrengthReduction.optimized.bc -o StrengthReduction.optimized.ll