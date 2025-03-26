cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR /Users/eliacapiluppi/Desktop/Assignment/AssignmentCompilatori/PrimoAssignment
opt -load-pass-plugin /Users/eliacapiluppi/Desktop/Assignment/AssignmentCompilatori/PrimoAssignment/build/libLocalOpts.dylib -p strength-reduction StrengthReduction.ll -o StrengthReduction.optimized.bc
llvm-dis StrengthReduction.optimized.bc -o StrengthReduction.optimized.ll