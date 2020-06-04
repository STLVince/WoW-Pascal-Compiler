; ModuleID = 'Pascal'
source_filename = "Pascal"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i8 39)
  ret void
}
