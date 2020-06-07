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

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 3, i32* @n, align 4
  store i32 10, i32* @v, align 4
  store i32 1, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @c, i64 0, i64 1), align 4
  store i32 2, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @w, i64 0, i64 1), align 4
  store i32 4, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @c, i64 0, i64 2), align 8
  store i32 6, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @w, i64 0, i64 2), align 8
  store i32 6, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @c, i64 0, i64 3), align 4
  store i32 8, i32* getelementptr inbounds ([101 x i32], [101 x i32]* @w, i64 0, i64 3), align 4
  br label %while

while:                                            ; preds = %RepeatExit, %entry
  %0 = phi i32 [ %.pre, %RepeatExit ], [ 3, %entry ]
  %1 = phi i32 [ %21, %RepeatExit ], [ 1, %entry ]
  store i32 %1, i32* @i, align 4
  %2 = icmp sgt i32 %1, %0
  br i1 %2, label %end1, label %loop

loop:                                             ; preds = %while
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i64 0, i64 0), i32 %1)
  %4 = load i32, i32* @v, align 4
  store i32 %4, i32* @j, align 4
  %.pre2 = load i32, i32* @i, align 4
  %.phi.trans.insert = sext i32 %.pre2 to i64
  %.phi.trans.insert3 = getelementptr inbounds [101 x i32], [101 x i32]* @c, i64 0, i64 %.phi.trans.insert
  %.pre4 = load i32, i32* %.phi.trans.insert3, align 4
  br label %RepeatStmt

RepeatStmt:                                       ; preds = %end, %loop
  %5 = phi i32 [ %23, %end ], [ %.pre2, %loop ]
  %6 = phi i32 [ %28, %end ], [ %.pre4, %loop ]
  %7 = phi i32 [ %25, %end ], [ %4, %loop ]
  %8 = sext i32 %5 to i64
  %9 = getelementptr inbounds [101 x i32], [101 x i32]* @c, i64 0, i64 %8
  %10 = sub i32 %7, %6
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i64 0, i64 %11
  %13 = load i32, i32* %12, align 4
  %14 = getelementptr inbounds [101 x i32], [101 x i32]* @w, i64 0, i64 %8
  %15 = load i32, i32* %14, align 4
  %16 = add i32 %15, %13
  %17 = sext i32 %7 to i64
  %18 = getelementptr inbounds [101 x i32], [101 x i32]* @f, i64 0, i64 %17
  %19 = load i32, i32* %18, align 4
  %20 = icmp sgt i32 %16, %19
  br i1 %20, label %then, label %else

RepeatExit:                                       ; preds = %end
  %21 = add i32 %23, 1
  %.pre = load i32, i32* @n, align 4
  br label %while

then:                                             ; preds = %RepeatStmt
  store i32 %16, i32* %18, align 4
  br label %end

else:                                             ; preds = %RepeatStmt
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i64 0, i64 0), i32 %19)
  %.pre5 = load i32, i32* @j, align 4
  %.pre6 = load i32, i32* @i, align 4
  br label %end

end:                                              ; preds = %else, %then
  %23 = phi i32 [ %.pre6, %else ], [ %5, %then ]
  %24 = phi i32 [ %.pre5, %else ], [ %7, %then ]
  %25 = add i32 %24, -1
  store i32 %25, i32* @j, align 4
  %26 = sext i32 %23 to i64
  %27 = getelementptr inbounds [101 x i32], [101 x i32]* @c, i64 0, i64 %26
  %28 = load i32, i32* %27, align 4
  %29 = add i32 %28, -1
  %30 = icmp eq i32 %25, %29
  br i1 %30, label %RepeatExit, label %RepeatStmt

end1:                                             ; preds = %while
  ret void
}
