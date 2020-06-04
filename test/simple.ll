; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 5, i32* @a
  ret void
}
