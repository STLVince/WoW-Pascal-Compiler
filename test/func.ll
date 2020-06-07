; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 @max(i32 1)
  store i32 %0, i32* @a
  %1 = load i32, i32* @a
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %1)
  %3 = call i32 @max(i32 3)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %3)
  %5 = call i32 @wow()
  ret void
}

define i32 @max(i32 %a1) {
entry:
  %a = alloca i32
  store i32 %a1, i32* %a
  %max = alloca i32
  %0 = load i32, i32* %a
  store i32 %0, i32* %max
  %1 = load i32, i32* %max
  ret i32 %1
}

define i32 @wow() {
entry:
  %wow = alloca i32
  store i32 5, i32* %wow
  %0 = load i32, i32* %wow
  ret i32 %0
}
