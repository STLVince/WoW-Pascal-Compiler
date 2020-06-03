; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0

define void @main() {
entry:
  store i32 5, i32* @a, align 4
  ret void
}
