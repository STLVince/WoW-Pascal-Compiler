; ModuleID = 'Pascal'
source_filename = "Pascal"

@a = global i32 0
@s = global i32 0

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store i32 1, i32* @a
  %0 = load i32, i32* @a
  %1 = icmp sgt i32 %0, 0
  br i1 %1, label %then, label %else

then:                                             ; preds = %entry
  store i32 1, i32* @s
  br label %end

else:                                             ; preds = %entry
  store i32 0, i32* @s
  br label %end

end:                                              ; preds = %else, %then
  ret void
}
