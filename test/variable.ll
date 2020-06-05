; ModuleID = 'Pascal'
source_filename = "Pascal"

@b = constant i32 1
@teststr = constant [6 x i8] c"'aaa'\00"
@a = global i32 0
@b.1 = global i32 0
@r = global double 0.000000e+00
@bool = global i1 false
@c = global i8 0
@s = global [1 x i8] zeroinitializer
@0 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store double 5.000000e-01, double* @r
  store i8 39, i8* @c
  %0 = load [6 x i8], [6 x i8]* @teststr
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), [6 x i8]* @teststr)
  ret void
}
