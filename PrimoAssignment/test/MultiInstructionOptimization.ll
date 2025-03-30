; ModuleID = 'MultiInstructionOptimization.bc'
source_filename = "MultiInstructionOptimization.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @multiInstructionOptimization(i32 noundef %0) #0 {
  %2 = add nsw i32 %0, 1
  %3 = sub nsw i32 %2, 1
  %4 = sub nsw i32 %0, 2
  %5 = add nsw i32 %4, 2
  %6 = mul nsw i32 3, %0
  %7 = sdiv i32 %6, 3
  %8 = sdiv i32 %0, 4
  %9 = mul nsw i32 4, %8
  %10 = add nsw i32 %3, %5
  %11 = add nsw i32 %10, %7
  %12 = add nsw i32 %11, %9
  ret i32 %12
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7"}
