; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@i = global i32 0
@j = global i32 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 2, i32* @a
  %0 = load i32, i32* @a
  %1 = load i32, i32* @a
  %2 = mul i32 %0, %1
  store i32 %2, i32* @i
  %3 = load i32, i32* @a
  %4 = load i32, i32* @a
  %5 = add i32 %3, %4
  store i32 %5, i32* @j
  ret void
}
