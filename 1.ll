; ModuleID = 'top'
source_filename = "top"

define i32 @main() {
entrypoint:
  %a = alloca i32
  store volatile i32 0, i32* %a
  br label %loop_cond

loop_cond:                                        ; preds = %loop_body, %entrypoint
  %tmp = load i32, i32* %a
  %ilt = icmp slt i32 %tmp, 10
  %i1toi32 = sext i1 %ilt to i32
  %condition = icmp ne i32 %i1toi32, 0
  br i1 %condition, label %loop_body, label %loop_exit

loop_body:                                        ; preds = %loop_cond
  %tmp1 = load i32, i32* %a
  %iadd = add i32 %tmp1, 1
  store volatile i32 %iadd, i32* %a
  br label %loop_cond

loop_exit:                                        ; preds = %loop_cond
  %b = alloca i32
  store volatile i32 10, i32* %b
  br label %loop_cond2

loop_cond2:                                       ; preds = %loop_body6, %loop_exit
  %tmp3 = load i32, i32* %b
  %igt = icmp sgt i32 %tmp3, 0
  %i1toi324 = sext i1 %igt to i32
  %condition5 = icmp ne i32 %i1toi324, 0
  br i1 %condition5, label %loop_body6, label %loop_exit10

loop_body6:                                       ; preds = %loop_cond2
  %tmp7 = load i32, i32* %b
  %isub = sub i32 %tmp7, 1
  store volatile i32 %isub, i32* %b
  %tmp8 = load i32, i32* %a
  %iadd9 = add i32 %tmp8, 1
  store volatile i32 %iadd9, i32* %a
  br label %loop_cond2

loop_exit10:                                      ; preds = %loop_cond2
  %tmp11 = load i32, i32* %a
  %tmp12 = load i32, i32* %b
  %isub13 = sub i32 %tmp11, %tmp12
  ret i32 %isub13
}
