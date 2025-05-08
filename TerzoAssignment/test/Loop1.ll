; ModuleID = 'LoopCodeM.c'
source_filename = "LoopCodeM.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

@.str = private unnamed_addr constant [25 x i8] c"%d,%d,%d,%d,%d,%d,%d,%d\0A\00", align 1

; Function Attrs: noinline nounwind ssp uwtable(sync)
define void @foo(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  store i32 3, ptr %5, align 4
  store i32 0, ptr %7, align 4
  store i32 3, ptr %8, align 4
  store i32 0, ptr %10, align 4
  store i32 0, ptr %11, align 4
  br label %12

12:                                               ; preds = %35, %2
  %13 = load i32, ptr %4, align 4
  %14 = add nsw i32 %13, 1
  store i32 %14, ptr %4, align 4
  %15 = load i32, ptr %3, align 4
  %16 = add nsw i32 %15, 3
  store i32 %16, ptr %11, align 4
  %17 = load i32, ptr %3, align 4
  %18 = add nsw i32 %17, 7
  store i32 %18, ptr %9, align 4
  %19 = load i32, ptr %4, align 4
  %20 = icmp slt i32 %19, 5
  br i1 %20, label %21, label %26

21:                                               ; preds = %12
  %22 = load i32, ptr %5, align 4
  %23 = add nsw i32 %22, 2
  store i32 %23, ptr %5, align 4
  %24 = load i32, ptr %3, align 4
  %25 = add nsw i32 %24, 3
  store i32 %25, ptr %6, align 4
  br label %35

26:                                               ; preds = %12
  %27 = load i32, ptr %5, align 4
  %28 = sub nsw i32 %27, 1
  store i32 %28, ptr %5, align 4
  %29 = load i32, ptr %3, align 4
  %30 = add nsw i32 %29, 4
  store i32 %30, ptr %6, align 4
  %31 = load i32, ptr %4, align 4
  %32 = icmp sge i32 %31, 6
  br i1 %32, label %33, label %34

33:                                               ; preds = %26
  br label %44

34:                                               ; preds = %26
  br label %35

35:                                               ; preds = %34, %21
  %36 = load i32, ptr %11, align 4
  %37 = add nsw i32 %36, 7
  store i32 %37, ptr %7, align 4
  %38 = load i32, ptr %6, align 4
  %39 = add nsw i32 %38, 2
  store i32 %39, ptr %8, align 4
  %40 = load i32, ptr %3, align 4
  %41 = add nsw i32 %40, 7
  store i32 %41, ptr %11, align 4
  %42 = load i32, ptr %9, align 4
  %43 = add nsw i32 %42, 2
  store i32 %43, ptr %10, align 4
  br label %12

44:                                               ; preds = %33
  %45 = load i32, ptr %5, align 4
  %46 = load i32, ptr %6, align 4
  %47 = load i32, ptr %7, align 4
  %48 = load i32, ptr %8, align 4
  %49 = load i32, ptr %9, align 4
  %50 = load i32, ptr %10, align 4
  %51 = load i32, ptr %11, align 4
  %52 = load i32, ptr %4, align 4
  %53 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %45, i32 noundef %46, i32 noundef %47, i32 noundef %48, i32 noundef %49, i32 noundef %50, i32 noundef %51, i32 noundef %52)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: noinline nounwind ssp uwtable(sync)
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  call void @foo(i32 noundef 0, i32 noundef 5)
  call void @foo(i32 noundef 0, i32 noundef 6)
  ret i32 0
}

attributes #0 = { noinline nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 1]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 19.1.7"}
