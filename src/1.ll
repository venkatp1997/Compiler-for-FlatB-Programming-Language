define internal void @main() {
entry:
  %a = alloca i32
  %b = alloca i32
  %c = alloca i32
  store i32 2, i32* %a
  ret void

  then:                                             ; preds = <null operand!>
  store i32 13, i32* %b
  br label %ifcont

  else:                                             ; preds = <null operand!>
    store i32 3, i32* %b
    br label %ifcont

    ifcont:                                           ; preds = %else, %then
    %iftmp = phi double [ <badref>, %then ], [ <badref>, %else ]
    %addtmp = fadd i32* %a, %b
    store i32* %addtmp, i32* %c
}

