; ModuleID = 'Pascal'
source_filename = "Pascal"

@cn = constant i32 2
@dn = constant double 1.232300e+02
@k = global i32 0
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  call void @outer()
  %0 = load i32, i32* @k
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %0)
  ret void
}

define void @outer() {
entry:
  %res = alloca i32
  %added = alloca i32
  store i32 0, i32* @k
  %0 = load i32, i32* @k
  call void @inner2(i32 %0, i32 10)
  ret void
}

define i32 @inner1(i32 %a1, i32 %b2) {
entry:
  %a = alloca i32
  store i32 %a1, i32* %a
  %b = alloca i32
  store i32 %b2, i32* %b
  %inner1 = alloca i32
  %0 = load i32, i32* %a
  %1 = load i32, i32* %b
  %2 = add i32 %0, %1
  store i32 %2, i32* %inner1
  %3 = load i32, i32* %inner1
  ret i32 %3
}

define void @inner2(i32 %aa1, i32 %b2) {
entry:
  %aa = alloca i32
  store i32 %aa1, i32* %aa
  %b = alloca i32
  store i32 %b2, i32* %b
  %0 = load i32, i32* %aa
  %1 = load i32, i32* %b
  %2 = call i32 @inner1(i32 %0, i32 %1)
  store i32 %2, i32* %aa
  %3 = load i32, i32* @k
  %4 = add i32 %3, 10
  store i32 %4, i32* @k
  ret void
}
