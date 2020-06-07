; ModuleID = 'Pascal'
source_filename = "Pascal"

@ta = constant i32 3
@c = global [11 x double] zeroinitializer
@i = global i32 0
@j = global i32 0
@k = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store double 1.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 0)
  store double 2.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 1)
  store double 3.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 2)
  store double 4.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 3)
  store double 5.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 4)
  store double 6.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 5)
  store double 0x3FE6666666666666, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 6)
  store double 8.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 7)
  store double 9.000000e-01, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 8)
  store double 1.000000e+00, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 9)
  store double 1.100000e+00, double* getelementptr inbounds ([11 x double], [11 x double]* @c, i32 0, i32 10)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), double 1.100000e+00)
  %1 = load i32, i32* @i
  store i32 0, i32* @i
  br label %while

while:                                            ; preds = %loop, %entry
  %2 = load i32, i32* @i
  %3 = icmp sle i32 %2, 10
  br i1 %3, label %loop, label %end

loop:                                             ; preds = %while
  %4 = load i32, i32* @i
  %5 = getelementptr inbounds [11 x double], [11 x double]* @c, i32 0, i32 %4
  %6 = load i32, i32* @i
  %7 = getelementptr inbounds [11 x double], [11 x double]* @c, i32 0, i32 %6
  %8 = load double, double* %7
  %9 = load i32, i32* @i
  %10 = getelementptr inbounds [11 x double], [11 x double]* @c, i32 0, i32 %9
  %11 = load double, double* %10
  %12 = fmul double %8, %11
  store double %12, double* %5
  %13 = load i32, i32* @i
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %13)
  %15 = load i32, i32* @i
  %16 = getelementptr inbounds [11 x double], [11 x double]* @c, i32 0, i32 %15
  %17 = load double, double* %16
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), double %17)
  %19 = load i32, i32* @i
  %20 = add i32 %19, 1
  store i32 %20, i32* @i
  br label %while

end:                                              ; preds = %while
  ret void
}
