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
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = load i32, i32* @a
  %1 = load i32, i32* @y
  %2 = add i32 %0, %1
  store i32 %2, i32* @e
  %3 = load i32, i32* @a
  %4 = load i32, i32* @y
  %5 = sub i32 %3, %4
  store i32 %5, i32* @g
  %6 = load i32, i32* @a
  %7 = load double, double* @b
  %8 = sitofp i32 %6 to double
  %9 = fmul double %8, %7
  store double %9, double* @f
  %10 = load i32, i32* @a
  %11 = load double, double* @b
  %12 = sitofp i32 %10 to double
  %13 = fdiv double %12, %11
  store double %13, double* @f
  %14 = load i32, i32* @a
  %15 = load double, double* @b
  %16 = sitofp i32 %14 to double
  %17 = fcmp oeq double %16, %15
  store i1 %17, i1* @i
  %18 = load i32, i32* @a
  %19 = load double, double* @b
  %20 = sitofp i32 %18 to double
  %21 = fcmp one double %20, %19
  store i1 %21, i1* @i
  %22 = load i32, i32* @a
  %23 = load double, double* @b
  %24 = sitofp i32 %22 to double
  %25 = fcmp ogt double %24, %23
  store i1 %25, i1* @i
  %26 = load i32, i32* @a
  %27 = load double, double* @b
  %28 = sitofp i32 %26 to double
  %29 = fcmp olt double %28, %27
  store i1 %29, i1* @i
  store double 2.200000e+00, double* @f
  store i32 3, i32* @g
  store i1 false, i1* @i
  %30 = load i32, i32* @a
  %31 = load i32, i32* @y
  %32 = and i32 %30, %31
  store i32 %32, i32* @e
  %33 = load i32, i32* @a
  %34 = load i32, i32* @y
  %35 = or i32 %33, %34
  store i32 %35, i32* @e
  %36 = load i32, i32* @a
  %37 = load i32, i32* @g
  %38 = sdiv i32 %36, %37
  store i32 %38, i32* @e
  %39 = load i32, i32* @a
  %40 = load i32, i32* @g
  %41 = srem i32 %39, %40
  store i32 %41, i32* @e
  store i32 0, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i32 0, i32 0)
  store i32 1, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i32 0, i32 1)
  store i32 2, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i32 0, i32 2)
  %42 = load i32, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @j, i32 0, i32 0)
  %43 = load i32, i32* @a
  %44 = add i32 %42, %43
  %45 = load i32, i32* @g
  %46 = sub i32 %44, %45
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %46)
  ret void
}

define i32 @test() {
entry:
  %test = alloca i32
  store i32 3, i32* %test
  %0 = load i32, i32* %test
  ret i32 %0
}
