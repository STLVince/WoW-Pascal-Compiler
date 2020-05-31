; ModuleID = 'main'
source_filename = "main"

@s = global i32 0
@i = global i32 0

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  store i32 0, i32* @s
  %0 = load i32, i32* @i
  store i32 1, i32* @i
  br label %while

while:                                            ; preds = %loop, %entry
  %1 = load i32, i32* @i
  %2 = icmp sle i32 %1, 10
  br i1 %2, label %loop, label %end

loop:                                             ; preds = %while
  %3 = load i32, i32* @s
  %4 = load i32, i32* @i
  %5 = add i32 %3, %4
  store i32 %5, i32* @s
  %6 = load i32, i32* @i
  %7 = add i32 %6, 1
  store i32 %7, i32* @i
  br label %while

end:                                              ; preds = %while
  ret i32 0
}
