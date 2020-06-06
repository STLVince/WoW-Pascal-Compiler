; ModuleID = 'Pascal'
source_filename = "Pascal"

@r = global double 0.000000e+00
@a = global [11 x i32] zeroinitializer
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 1, i32* getelementptr inbounds ([11 x i32], [11 x i32]* @a, i32 0, i32 1)
  store i32 3, i32* getelementptr inbounds ([11 x i32], [11 x i32]* @a, i32 0, i32 3)
  %0 = load i32, i32* getelementptr inbounds ([11 x i32], [11 x i32]* @a, i32 0, i32 3)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %0)
  ret void
}
