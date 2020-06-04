; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 @fib(i32 5)
  store i32 %0, i32* @a
  %1 = load i32, i32* @a
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %1)
  ret void
}

define i32 @fib(i32 %n1) {
entry:
  %n = alloca i32
  store i32 %n1, i32* %n
  %fib = alloca i32
  %0 = load i32, i32* %n
  %1 = icmp sle i32 %0, 2
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  store i32 1, i32* %fib
  br label %end

else:                                             ; preds = %entry
  %2 = load i32, i32* %n
  %3 = sub i32 %2, 1
  %4 = call i32 @fib(i32 %3)
  %5 = load i32, i32* %n
  %6 = sub i32 %5, 2
  %7 = call i32 @fib(i32 %6)
  %8 = add i32 %4, %7
  store i32 %8, i32* %fib
  br label %end

end:                                              ; preds = %else, %then
  %9 = load i32, i32* %fib
  ret i32 %9
}
