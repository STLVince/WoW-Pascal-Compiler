; ModuleID = 'Pascal'
source_filename = "Pascal"

@y = constant i32 33
@a = constant i32 1
@b = constant double 2.200000e+00
@c = constant [7 x i8] c"string\00"
@d = constant i1 false
@e = global i32 0
@f = global double 0.000000e+00
@g = global i32 0
@h = global [1 x i8] zeroinitializer
@i = global i1 false
@j = global [3 x i32] zeroinitializer
@k = global i32 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 34, i32* @e, align 4
  store i32 -32, i32* @g, align 4
  store double 0x3FDD1745D1745D17, double* @f, align 8
  store i1 true, i1* @i, align 1
  store double 2.200000e+00, double* @f, align 8
  store i32 3, i32* @g, align 4
  store i1 false, i1* @i, align 1
  store i32 1, i32* @e, align 4
  store i32 0, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i64 0, i64 0), align 4
  store i32 1, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i64 0, i64 1), align 4
  store i32 2, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i64 0, i64 2), align 4
  ret void
}

define i32 @test() {
entry:
  ret i32 3
}
