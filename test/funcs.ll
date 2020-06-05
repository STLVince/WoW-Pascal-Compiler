; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 @f(i32 4)
  store i32 %0, i32* @a
  %1 = load i32, i32* @a
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %1)
  ret void
}

define i32 @f(i32 %a1) {
entry:
  %a = alloca i32
  store i32 %a1, i32* %a
  %f = alloca i32
  %0 = load i32, i32* %a
  %1 = call i32 @g(i32 %0)
  %2 = add i32 %1, 2
  store i32 %2, i32* %f
  %3 = load i32, i32* %f
  ret i32 %3
}

define i32 @g(i32 %b1) {
entry:
  %b = alloca i32
  store i32 %b1, i32* %b
  %g = alloca i32
  %0 = load i32, i32* %b
  %1 = mul i32 %0, 3
  store i32 %1, i32* %g
  %2 = load i32, i32* %g
  ret i32 %2
}
