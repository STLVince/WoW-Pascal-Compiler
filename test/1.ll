; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@i = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = load i32, i32* @i
  store i32 1, i32* @i
  br label %while

while:                                            ; preds = %loop, %entry
  %1 = load i32, i32* @i
  %2 = icmp sle i32 %1, 10
  br i1 %2, label %loop, label %end

loop:                                             ; preds = %while
  %3 = load i32, i32* @i
  %4 = call i32 @fib(i32 %3)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %4)
  %6 = load i32, i32* @i
  %7 = add i32 %6, 1
  store i32 %7, i32* @i
  br label %while

end:                                              ; preds = %while
  ret void
}

define i32 @fib(i32 %x1) {
entry:
  %x = alloca i32
  store i32 %x1, i32* %x
  %fib = alloca i32
  %0 = load i32, i32* %x
  %1 = icmp eq i32 %0, 0
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  store i32 0, i32* %fib
  br label %end4

else:                                             ; preds = %entry
  %2 = load i32, i32* %x
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %then2, label %else3

then2:                                            ; preds = %else
  store i32 1, i32* %fib
  br label %end

else3:                                            ; preds = %else
  %4 = load i32, i32* %x
  %5 = sub i32 %4, 1
  %6 = call i32 @fib(i32 %5)
  %7 = load i32, i32* %x
  %8 = sub i32 %7, 2
  %9 = call i32 @fib(i32 %8)
  %10 = add i32 %6, %9
  store i32 %10, i32* %fib
  br label %end

end:                                              ; preds = %else3, %then2
  br label %end4

end4:                                             ; preds = %end, %then
  %11 = load i32, i32* %fib
  ret i32 %11
}
