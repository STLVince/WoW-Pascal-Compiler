; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@b = global i32 0
@r = global double 0.000000e+00
@bool = global i1 false
@c = global i8 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store double 5.000000e-01, double* @r
  store i8 39, i8* @c
  ret void
}
