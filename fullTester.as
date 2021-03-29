MAIN: .data 9, -6
DATA: .data 97 , +56
STR: .string "T"

;nothing!
.extern extern
.entry MAIN

    mov extern, r1
.entry      STR
or1: prn     #48
    or2:     jmp %DATA
