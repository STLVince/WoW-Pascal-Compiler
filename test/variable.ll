; ModuleID = 'Pascal'
source_filename = "Pascal"

@b = constant i32 1
@teststr = constant [4 x i8] c"aaa\00"
@a = global i32 0
@b.1 = global i32 0
@r = global double 0.000000e+00
@bool = global i1 false
@c = global i8 0
@s = global [1 x i8] zeroinitializer
@x = global [3 x i32] zeroinitializer
@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = constant [3 x i8] c"aa\00"
@4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store double -5.000000e-01, double* @r
  store i8 48, i8* @c
  store i32 -2, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @x, i32 0, i32 2)
  %0 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @x, i32 0, i32 1)
  store i32 %0, i32* @a
  %1 = load i32, i32* @b
  %2 = icmp eq i32 %1, 1
  br i1 %2, label %then, label %else

then:                                             ; preds = %entry
  store i32 6, i32* @a
  br label %end

else:                                             ; preds = %entry
  store i32 9, i32* @a
  br label %end

end:                                              ; preds = %else, %then
  %3 = load double, double* @r
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), double %3)
  %5 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @x, i32 0, i32 2)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %5)
  %7 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @x, i32 0, i32 0)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %7)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @4, i32 0, i32 0), [3 x i8]* @3)
  %10 = load [4 x i8], [4 x i8]* @teststr
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @5, i32 0, i32 0), [4 x i8]* @teststr)
  ret void
}
