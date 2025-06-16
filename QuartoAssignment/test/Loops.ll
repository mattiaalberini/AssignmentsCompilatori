; ModuleID = 'Loops.app.bc'
source_filename = "Loops.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@z = dso_local global i32 9, align 4

; Function Attrs: noinline nounwind uwtable
define dso_local void @loops(ptr noundef %0, ptr noundef %1, ptr noundef %2, ptr noundef %3, i32 noundef %4, i32 noundef %5) #0 {
  br label %7

7:                                                ; preds = %12, %6
  %.0 = phi i32 [ 0, %6 ], [ %13, %12 ]
  %8 = icmp slt i32 %.0, 4
  br i1 %8, label %9, label %14

9:                                                ; preds = %7
  %10 = sext i32 %.0 to i64
  %11 = getelementptr inbounds i32, ptr %0, i64 %10
  store i32 3, ptr %11, align 4
  br label %12

12:                                               ; preds = %9
  %13 = add nsw i32 %.0, 1
  br label %7, !llvm.loop !6

14:                                               ; preds = %7
  br label %15

15:                                               ; preds = %22, %14
  %.1 = phi i32 [ 0, %14 ], [ %23, %22 ]
  %16 = icmp slt i32 %.1, 4
  br i1 %16, label %17, label %24

17:                                               ; preds = %15
  %18 = sext i32 %.1 to i64
  %19 = getelementptr inbounds i32, ptr %0, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = add nsw i32 %20, 2
  br label %22

22:                                               ; preds = %17
  %23 = add nsw i32 %.1, 1
  br label %15, !llvm.loop !8

24:                                               ; preds = %15
  ret void
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
!8 = distinct !{!8, !7}
