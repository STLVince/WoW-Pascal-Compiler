; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = constant i32 1
@str = constant [6 x i8] c"hello\00"
@b = global i32 0
@c = global i32 0
@d = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 1, i32* @d
  %0 = load i32, i32* @a
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %0)
  %2 = load [6 x i8], [6 x i8]* @str
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), [4 x i8] c"aa\0A\00")
  ret void
}
