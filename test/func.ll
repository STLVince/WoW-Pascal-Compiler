; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  %0 = call i32 @max(i32 1)
  store i32 %0, i32* @a
  ret void
}

define i32 @max(i32 %a1) {
entry:
  %a = alloca i32
  store i32 %a1, i32* %a
  %max = alloca i32
  %0 = load i32, i32* %a
  store i32 %0, i32* %max
  %1 = load i32, i32* %max
  ret i32 %1
}
