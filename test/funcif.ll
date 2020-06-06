; ModuleID = 'Pascal'
source_filename = "Pascal"

@i = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 @go(i32 10)
  store i32 %0, i32* @i
  %1 = load i32, i32* @i
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %1)
  ret void
}

define i32 @go(i32 %a1) {
entry:
  %a = alloca i32
  store i32 %a1, i32* %a
  %go = alloca i32
  %0 = load i32, i32* %a
  %1 = icmp eq i32 %0, 1
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  store i32 1, i32* %go
  br label %end4

else:                                             ; preds = %entry
  %2 = load i32, i32* %a
  %3 = icmp eq i32 %2, 2
  br i1 %3, label %then2, label %else3

then2:                                            ; preds = %else
  store i32 1, i32* %go
  br label %end

else3:                                            ; preds = %else
  %4 = load i32, i32* %a
  %5 = sub i32 %4, 1
  %6 = call i32 @go(i32 %5)
  %7 = load i32, i32* %a
  %8 = sub i32 %7, 2
  %9 = call i32 @go(i32 %8)
  %10 = add i32 %6, %9
  store i32 %10, i32* %go
  br label %end

end:                                              ; preds = %else3, %then2
  br label %end4

end4:                                             ; preds = %end, %then
  %11 = load i32, i32* %go
  ret i32 %11
}
