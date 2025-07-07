; ModuleID = 'LoopTest.app.bc'
source_filename = "LoopTest.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @loop(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3, i32 noundef %4, i32 noundef %5) #0 {
  br label %7

7:                                                ; preds = %20, %6
  %.01 = phi i32 [ 0, %6 ], [ %19, %20 ]
  %.0 = phi i32 [ undef, %6 ], [ %17, %20 ]
  %8 = add nsw i32 %3, 1
  %9 = add nsw i32 %3, 2
  %10 = icmp slt i32 %3, %5
  br i1 %10, label %11, label %16

11:                                               ; preds = %7
  %12 = add nsw i32 %3, 3
  %13 = icmp slt i32 %4, %5
  br i1 %13, label %14, label %15

14:                                               ; preds = %11
  br label %22

15:                                               ; preds = %11
  br label %16

16:                                               ; preds = %15, %7
  %17 = add nsw i32 %3, 4
  %18 = add nsw i32 %3, 5
  %19 = add nsw i32 %.01, 1
  br label %20

20:                                               ; preds = %16
  %21 = icmp slt i32 %19, %5
  br i1 %21, label %7, label %22, !llvm.loop !6

22:                                               ; preds = %20, %14
  %.1 = phi i32 [ %.0, %14 ], [ %17, %20 ]
  %23 = add nsw i32 %9, 6
  %24 = add nsw i32 %.1, 7
  ret i32 %23
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
