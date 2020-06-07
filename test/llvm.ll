; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@b = global i32 0
@c = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 1, i32* @a
  br label %while

while:                                            ; preds = %loop, %entry
  %0 = load i32, i32* @a
  %1 = icmp slt i32 %0, 30
  br i1 %1, label %loop, label %end

loop:                                             ; preds = %while
  %2 = load i32, i32* @a
  %3 = add i32 %2, 1
  store i32 %3, i32* @a
  %4 = load i32, i32* @a
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %4)
  br label %while

end:                                              ; preds = %while
  br label %RepeatStmt

RepeatStmt:                                       ; preds = %RepeatEnd, %end
  %6 = load i32, i32* @a
  %7 = add i32 %6, 1
  store i32 %7, i32* @a
  br label %RepeatEnd

RepeatEnd:                                        ; preds = %RepeatStmt
  %8 = load i32, i32* @a
  %9 = icmp eq i32 %8, 60
  br i1 %9, label %RepeatExit, label %RepeatStmt

RepeatExit:                                       ; preds = %RepeatEnd
  %10 = load i32, i32* @a
  store i32 1, i32* @a
  br label %while1

while1:                                           ; preds = %loop2, %RepeatExit
  %11 = load i32, i32* @a
  %12 = icmp sle i32 %11, 100
  br i1 %12, label %loop2, label %end3

loop2:                                            ; preds = %while1
  %13 = load i32, i32* @b
  %14 = add i32 %13, 1
  store i32 %14, i32* @b
  %15 = load i32, i32* @b
  %16 = add i32 %15, 1
  store i32 %16, i32* @b
  %17 = load i32, i32* @a
  %18 = add i32 %17, 1
  store i32 %18, i32* @a
  br label %while1

end3:                                             ; preds = %while1
  %19 = load i32, i32* @b
  %20 = icmp sgt i32 %19, 30
  %21 = load i32, i32* @b
  %22 = icmp slt i32 %21, 100
  %23 = and i1 %20, %22
  br i1 %23, label %then, label %else6

then:                                             ; preds = %end3
  %24 = load i32, i32* @a
  %25 = icmp slt i32 %24, 120
  br i1 %25, label %then4, label %else

then4:                                            ; preds = %then
  %26 = load i32, i32* @a
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %26)
  br label %end5

else:                                             ; preds = %then
  br label %end5

end5:                                             ; preds = %else, %then4
  br label %end7

else6:                                            ; preds = %end3
  br label %end7

end7:                                             ; preds = %else6, %end5
  store i32 1, i32* @a
  %28 = load i32, i32* @a
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %28)
  %30 = load i32, i32* @a
  %31 = icmp eq i32 %30, 1
  br i1 %31, label %caseStmt, label %next

exit:                                             ; preds = %next
  %32 = load i32, i32* @b
  %33 = mul i32 %32, 3
  %34 = srem i32 %33, 600
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @5, i32 0, i32 0), i32 %34)
  ret void

caseStmt:                                         ; preds = %next, %end7

caseStmt8:                                        ; preds = %next, %next

next:                                             ; preds = %end7
  %36 = load i32, i32* @a
  %37 = icmp eq i32 %36, 100
  br i1 %37, label %caseStmt8, label %exit
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @3, i32 0, i32 0), i32 1)
  br label %caseStmt
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @4, i32 0, i32 0), i32 9999)
  br label %caseStmt8
}

define void @count(i32 %b1) {
entry:
  %b = alloca i32
  store i32 %b1, i32* %b
  store i32 1, i32* %b
  ret void
}
