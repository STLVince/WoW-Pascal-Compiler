; ModuleID = 'Pascal'
source_filename = "Pascal"

@f = global i32 0
@k = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 0, i32* @k
  %0 = load i32, i32* @k
  %1 = call i32 @go(i32 %0, i32 5)
  store i32 %1, i32* @f
  %2 = load i32, i32* @f
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %2)
  %4 = load i32, i32* @k
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %4)
  ret void
}

define i32 @go(i32 %b1, i32 %a2) {
entry:
  %b = alloca i32
  store i32 %b1, i32* %b
  %a = alloca i32
  store i32 %a2, i32* %a
  %fk = alloca i32
  %t = alloca double
  %go = alloca i32
  %0 = load i32, i32* %a
  %1 = icmp sgt i32 %0, 0
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  %2 = load i32, i32* %a
  %3 = load i32, i32* %b
  %4 = load i32, i32* %a
  %5 = sub i32 %4, 1
  %6 = call i32 @go(i32 %3, i32 %5)
  %7 = mul i32 %2, %6
  store i32 %7, i32* %go
  br label %end

else:                                             ; preds = %entry
  store i32 1, i32* %go
  br label %end

end:                                              ; preds = %else, %then
  %8 = load i32, i32* %b
  %9 = load i32, i32* %go
  %10 = add i32 %8, %9
  store i32 %10, i32* %b
  %11 = load i32, i32* @k
  %12 = load i32, i32* %go
  %13 = add i32 %11, %12
  store i32 %13, i32* @k
  %14 = load i32, i32* %go
  ret i32 %14
}
