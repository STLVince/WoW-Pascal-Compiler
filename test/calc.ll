; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@i = global i32 0
@j = global i32 0

define void @main() {
entry:
  store i32 2, i32* @a, align 4
  store i32 4, i32* @i, align 4
  store i32 4, i32* @j, align 4
  ret void
}
