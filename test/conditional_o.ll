; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@s = global i32 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 1, i32* @a, align 4
  store i32 1, i32* @s, align 4
  ret void
}
