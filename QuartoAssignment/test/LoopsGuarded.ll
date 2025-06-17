; ModuleID = 'LoopsGuarded.app.bc'
source_filename = "LoopsGuarded.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @loops(ptr noundef %0, ptr noundef %1, i32 noundef %2) #0 {
  %4 = icmp sgt i32 4, 2
  br i1 %4, label %5, label %14

5:                                                ; preds = %3
  br label %6

6:                                                ; preds = %11, %5
  %.0 = phi i32 [ 0, %5 ], [ %10, %11 ]
  %7 = add nsw i32 %.0, 3
  %8 = sext i32 %.0 to i64
  %9 = getelementptr inbounds i32, ptr %0, i64 %8
  store i32 %7, ptr %9, align 4
  %10 = add nsw i32 %.0, 1
  br label %11

11:                                               ; preds = %6
  %12 = icmp slt i32 %10, 4
  br i1 %12, label %6, label %13, !llvm.loop !6

13:                                               ; preds = %11
  br label %14

14:                                               ; preds = %13, %3
  %15 = icmp sgt i32 4, 2
  br i1 %15, label %16, label %28

16:                                               ; preds = %14
  br label %17

17:                                               ; preds = %25, %16
  %.1 = phi i32 [ 0, %16 ], [ %24, %25 ]
  %18 = sext i32 %.1 to i64
  %19 = getelementptr inbounds i32, ptr %0, i64 %18
  %20 = load i32, ptr %19, align 4
  %21 = sub nsw i32 %20, 1
  %22 = sext i32 %.1 to i64
  %23 = getelementptr inbounds i32, ptr %1, i64 %22
  store i32 %21, ptr %23, align 4
  %24 = add nsw i32 %.1, 1
  br label %25

25:                                               ; preds = %17
  %26 = icmp slt i32 %24, 4
  br i1 %26, label %17, label %27, !llvm.loop !8

27:                                               ; preds = %25
  br label %28

28:                                               ; preds = %27, %14
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
