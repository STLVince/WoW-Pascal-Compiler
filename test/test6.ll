; ModuleID = 'Pascal'
source_filename = "Pascal"

@ans = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 @gcd(i32 9, i32 36)
  %1 = call i32 @gcd(i32 3, i32 6)
  %2 = mul i32 %0, %1
  store i32 %2, i32* @ans
  %3 = load i32, i32* @ans
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %3)
  ret void
}

define i32 @gcd(i32 %a1, i32 %b2) {
entry:
  %a = alloca i32
  store i32 %a1, i32* %a
  %b = alloca i32
  store i32 %b2, i32* %b
  %gcd = alloca i32
  %0 = load i32, i32* %b
  %1 = icmp eq i32 %0, 0
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  %2 = load i32, i32* %a
  store i32 %2, i32* %gcd
  br label %end

else:                                             ; preds = %entry
  %3 = load i32, i32* %b
  %4 = load i32, i32* %a
  %5 = load i32, i32* %b
  %6 = srem i32 %4, %5
  %7 = call i32 @gcd(i32 %3, i32 %6)
  store i32 %7, i32* %gcd
  br label %end

end:                                              ; preds = %else, %then
  %8 = load i32, i32* %gcd
  ret i32 %8
}
