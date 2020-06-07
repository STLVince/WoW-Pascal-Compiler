; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 5, i32* @a
  call void @pr()
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  ret void
}

define void @pr() {
entry:
  %0 = load i32, i32* @a
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %0)
  ret void
}
