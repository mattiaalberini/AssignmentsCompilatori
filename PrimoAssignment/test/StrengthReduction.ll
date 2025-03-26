define dso_local i32 @strengthReduction(i32 noundef %0, i32 noundef %1) #0 {
  %3 = mul nsw i32 15, %1
  %4 = mul nsw i32 %1, 9
  %5 = add nsw i32 %4, %3
  ret i32 %5
}