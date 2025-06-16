; ModuleID = 'LoopsGuarded.app.bc'
source_filename = "LoopsGuarded.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @loops(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = icmp sgt i32 4, 2
  br i1 %4, label %5, label %13

5:                                                ; preds = %3
  br label %6

6:                                                ; preds = %10, %5
  %.0 = phi i32 [ 0, %5 ], [ %9, %10 ]
  %7 = sext i32 %.0 to i64
  %8 = getelementptr inbounds i32, ptr %0, i64 %7
  store i32 3, ptr %8, align 4
  %9 = add nsw i32 %.0, 1
  br label %10

10:                                               ; preds = %6
  %11 = icmp slt i32 %9, 4
  br i1 %11, label %6, label %12, !llvm.loop !6

12:                                               ; preds = %10
  br label %13

13:                                               ; preds = %12, %3
  %14 = icmp sgt i32 4, 2
  br i1 %14, label %15, label %23

15:                                               ; preds = %13
  br label %16

16:                                               ; preds = %20, %15
  %.1 = phi i32 [ 0, %15 ], [ %19, %20 ]
  %17 = sext i32 %.1 to i64
  %18 = getelementptr inbounds i32, ptr %1, i64 %17
  store i32 2, ptr %18, align 4
  %19 = add nsw i32 %.1, 1
  br label %20

20:                                               ; preds = %16
  %21 = icmp slt i32 %19, 4
  br i1 %21, label %16, label %22, !llvm.loop !8

22:                                               ; preds = %20
  br label %23

23:                                               ; preds = %22, %13
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
