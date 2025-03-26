define dso_local i32 @strengthReduction(i32 noundef %0, i32 noundef %1) #0 {
  %3 = mul nsw i32 15, %1
  %4 = mul nsw i32 %1, 9
  %5 = add nsw i32 %4, %3
  %6 = sdiv i32 %5, 4
  ret i32 %6
}