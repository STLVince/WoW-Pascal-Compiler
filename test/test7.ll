; ModuleID = 'Pascal'
source_filename = "Pascal"

@c = global [11 x i32] zeroinitializer
@d = global [11 x i32] zeroinitializer
@i = global i32 0
@aaa = global i32 0
@bbb = global i32 0
@ccc = global i32 0
@ddd = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = load i32, i32* @i
  store i32 0, i32* @i
  br label %while

while:                                            ; preds = %loop, %entry
  %1 = load i32, i32* @i
  %2 = icmp sle i32 %1, 10
  br i1 %2, label %loop, label %end

loop:                                             ; preds = %while
  %3 = load i32, i32* @i
  %4 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %3
  %5 = load i32, i32* @i
  store i32 %5, i32* %4
  %6 = load i32, i32* @i
  %7 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %6
  %8 = load i32, i32* @i
  %9 = sub i32 10, %8
  store i32 %9, i32* %7
  %10 = load i32, i32* @i
  %11 = add i32 %10, 1
  store i32 %11, i32* @i
  br label %while

end:                                              ; preds = %while
  store i32 1, i32* @aaa
  %12 = load i32, i32* @i
  store i32 0, i32* @i
  br label %while1

while1:                                           ; preds = %loop2, %end
  %13 = load i32, i32* @i
  %14 = icmp sle i32 %13, 10
  br i1 %14, label %loop2, label %end3

loop2:                                            ; preds = %while1
  %15 = load i32, i32* @i
  %16 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %15
  %17 = load i32, i32* @i
  %18 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %17
  %19 = load i32, i32* %18
  %20 = load i32, i32* @i
  %21 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %20
  %22 = load i32, i32* %21
  %23 = mul i32 %19, %22
  store i32 %23, i32* %16
  %24 = load i32, i32* @i
  %25 = add i32 %24, 1
  store i32 %25, i32* @i
  br label %while1

end3:                                             ; preds = %while1
  store i32 1, i32* @bbb
  %26 = load i32, i32* @i
  store i32 0, i32* @i
  br label %while4

while4:                                           ; preds = %end6, %end3
  %27 = load i32, i32* @i
  %28 = icmp sle i32 %27, 10
  br i1 %28, label %loop5, label %end7

loop5:                                            ; preds = %while4
  %29 = load i32, i32* @i
  %30 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %29
  %31 = load i32, i32* %30
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %31)
  %33 = load i32, i32* @i
  %34 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %33
  %35 = load i32, i32* %34
  %36 = icmp eq i32 %35, 2
  br i1 %36, label %then, label %else

then:                                             ; preds = %loop5
  %37 = load i32, i32* @i
  %38 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %37
  store i32 1, i32* %38
  br label %end6

else:                                             ; preds = %loop5
  br label %end6

end6:                                             ; preds = %else, %then
  %39 = load i32, i32* @i
  %40 = add i32 %39, 1
  store i32 %40, i32* @i
  br label %while4

end7:                                             ; preds = %while4
  store i32 1, i32* @ccc
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 11112)
  %42 = load i32, i32* @i
  store i32 0, i32* @i
  br label %while8

while8:                                           ; preds = %loop9, %end7
  %43 = load i32, i32* @i
  %44 = icmp sle i32 %43, 10
  br i1 %44, label %loop9, label %end10

loop9:                                            ; preds = %while8
  %45 = load i32, i32* @i
  %46 = getelementptr inbounds [11 x i32], [11 x i32]* @c, i32 0, i32 %45
  %47 = load i32, i32* %46
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %47)
  %49 = load i32, i32* @i
  %50 = add i32 %49, 1
  store i32 %50, i32* @i
  br label %while8

end10:                                            ; preds = %while8
  store i32 1, i32* @ddd
  ret void
}
