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

label:                                            ; preds = %exit, %caseStmt8
  %0 = load i32, i32* @b
  %1 = mul i32 %0, 3
  %2 = srem i32 %1, 6
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @5, i32 0, i32 0), i32 %2)
  store i32 1, i32* @a
  ret void

while:                                            ; preds = %loop, %entry
  %4 = load i32, i32* @a
  %5 = icmp slt i32 %4, 30
  br i1 %5, label %loop, label %end

loop:                                             ; preds = %while
  %6 = load i32, i32* @a
  %7 = add i32 %6, 1
  store i32 %7, i32* @a
  %8 = load i32, i32* @a
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %8)
  br label %while

end:                                              ; preds = %while
  br label %RepeatStmt

RepeatStmt:                                       ; preds = %RepeatEnd, %end
  %10 = load i32, i32* @a
  %11 = add i32 %10, 1
  store i32 %11, i32* @a
  br label %RepeatEnd

RepeatEnd:                                        ; preds = %RepeatStmt
  %12 = load i32, i32* @a
  %13 = icmp eq i32 %12, 60
  br i1 %13, label %RepeatExit, label %RepeatStmt

RepeatExit:                                       ; preds = %RepeatEnd
  %14 = load i32, i32* @a
  store i32 1, i32* @a
  br label %while1

while1:                                           ; preds = %loop2, %RepeatExit
  %15 = load i32, i32* @a
  %16 = icmp sle i32 %15, 100
  br i1 %16, label %loop2, label %end3

loop2:                                            ; preds = %while1
  %17 = load i32, i32* @b
  %18 = add i32 %17, 1
  store i32 %18, i32* @b
  %19 = load i32, i32* @b
  %20 = add i32 %19, 1
  store i32 %20, i32* @b
  %21 = load i32, i32* @a
  %22 = add i32 %21, 1
  store i32 %22, i32* @a
  br label %while1

end3:                                             ; preds = %while1
  %23 = load i32, i32* @b
  %24 = icmp sgt i32 %23, 30
  %25 = load i32, i32* @b
  %26 = icmp slt i32 %25, 100
  %27 = and i1 %24, %26
  br i1 %27, label %then, label %else6

then:                                             ; preds = %end3
  %28 = load i32, i32* @a
  %29 = icmp slt i32 %28, 120
  br i1 %29, label %then4, label %else

then4:                                            ; preds = %then
  %30 = load i32, i32* @a
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %30)
  br label %end5

else:                                             ; preds = %then
  br label %end5

end5:                                             ; preds = %else, %then4
  br label %end7

else6:                                            ; preds = %end3
  br label %end7

end7:                                             ; preds = %else6, %end5
  store i32 1, i32* @a
  %32 = load i32, i32* @a
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %32)
  %34 = load i32, i32* @a
  %35 = icmp eq i32 %34, 1
  br i1 %35, label %caseStmt, label %next

exit:                                             ; preds = %afterGoto, %caseStmt, %next
  br label %label

caseStmt:                                         ; preds = %end7
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @3, i32 0, i32 0), i32 1)
  br label %exit

caseStmt8:                                        ; preds = %next
  br label %label

next:                                             ; preds = %end7
  %37 = load i32, i32* @a
  %38 = icmp eq i32 %37, 100
  br i1 %38, label %caseStmt8, label %exit

afterGoto:                                        ; No predecessors!
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @4, i32 0, i32 0), i32 9999)
  br label %exit
}

define void @count(i32 %b1) {
entry:
  %b = alloca i32
  store i32 %b1, i32* %b
  store i32 1, i32* %b
  ret void
}
