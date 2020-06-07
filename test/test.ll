; ModuleID = 'Pascal'
source_filename = "Pascal"

@i = global i32 0
@j = global i32 0
@n = global i32 0
@v = global i32 0
@c = global [101 x i32] zeroinitializer
@w = global [101 x i32] zeroinitializer
@f = global [101 x i32] zeroinitializer
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 3, i32* @n
  store i32 10, i32* @v
  store i32 1, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @c, i32 0, i32 1)
  store i32 2, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @w, i32 0, i32 1)
  store i32 4, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @c, i32 0, i32 2)
  store i32 6, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @w, i32 0, i32 2)
  store i32 6, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @c, i32 0, i32 3)
  store i32 8, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @w, i32 0, i32 3)
  %0 = load i32, i32* @i
  store i32 1, i32* @i
  br label %while

while:                                            ; preds = %RepeatExit, %entry
  %1 = load i32, i32* @i
  %2 = load i32, i32* @n
  %3 = icmp sle i32 %1, %2
  br i1 %3, label %loop, label %end1

loop:                                             ; preds = %while
  %4 = load i32, i32* @i
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %4)
  %6 = load i32, i32* @v
  store i32 %6, i32* @j
  br label %RepeatStmt

RepeatStmt:                                       ; preds = %RepeatEnd, %loop
  %7 = load i32, i32* @j
  %8 = load i32, i32* @i
  %9 = getelementptr inbounds [101 x i32], [101 x i32]* @c, i32 0, i32 %8
  %10 = load i32, i32* %9
  %11 = sub i32 %7, %10
  %12 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i32 0, i32 %11
  %13 = load i32, i32* %12
  %14 = load i32, i32* @i
  %15 = getelementptr inbounds [101 x i32], [101 x i32]* @w, i32 0, i32 %14
  %16 = load i32, i32* %15
  %17 = add i32 %13, %16
  %18 = load i32, i32* @j
  %19 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i32 0, i32 %18
  %20 = load i32, i32* %19
  %21 = icmp sgt i32 %17, %20
  br i1 %21, label %then, label %else

RepeatEnd:                                        ; preds = %end
  %22 = load i32, i32* @j
  %23 = load i32, i32* @i
  %24 = getelementptr inbounds [101 x i32], [101 x i32]* @c, i32 0, i32 %23
  %25 = load i32, i32* %24
  %26 = sub i32 %25, 1
  %27 = icmp eq i32 %22, %26
  br i1 %27, label %RepeatExit, label %RepeatStmt

RepeatExit:                                       ; preds = %RepeatEnd
  %28 = load i32, i32* @i
  %29 = add i32 %28, 1
  store i32 %29, i32* @i
  br label %while

then:                                             ; preds = %RepeatStmt
  %30 = load i32, i32* @j
  %31 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i32 0, i32 %30
  %32 = load i32, i32* @j
  %33 = load i32, i32* @i
  %34 = getelementptr inbounds [101 x i32], [101 x i32]* @c, i32 0, i32 %33
  %35 = load i32, i32* %34
  %36 = sub i32 %32, %35
  %37 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i32 0, i32 %36
  %38 = load i32, i32* %37
  %39 = load i32, i32* @i
  %40 = getelementptr inbounds [101 x i32], [101 x i32]* @w, i32 0, i32 %39
  %41 = load i32, i32* %40
  %42 = add i32 %38, %41
  store i32 %42, i32* %31
  br label %end

else:                                             ; preds = %RepeatStmt
  %43 = load i32, i32* @j
  %44 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i32 0, i32 %43
  %45 = load i32, i32* %44
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %45)
  br label %end

end:                                              ; preds = %else, %then
  %47 = load i32, i32* @j
  %48 = sub i32 %47, 1
  store i32 %48, i32* @j
  br label %RepeatEnd

end1:                                             ; preds = %while
  ret void
}
