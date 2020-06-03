; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@b = global i32 0
@r = global double 0.000000e+00
@bool = global i1 false
@c = global [1 x i8] zeroinitializer
@s = global [1 x i8] zeroinitializer

define void @main() {
entry:
  store double 5.000000e-01, double* @r, align 8
  ret void
}
