; ModuleID = 'main'
source_filename = "main"

@a = global i32 0
@max_a = global i32 0
@max_max = global i32 0

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = call i32 @max(i32 1)
  store i32 %0, i32* @max_a
  ret i32 0
}

define i32 @max(i32) {
entry:
  store i32 %0, i32* @max_a
  %1 = load i32, i32* @max_a
  store i32 %1, i32* @max_max
  %2 = load i32, i32* @max_max
  ret i32 %2
}
