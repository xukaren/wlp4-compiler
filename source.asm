
;<------------------ wain ---------------->

;---- wain prologue -----
.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $10
.word print
lis $11
.word 1
sub $29, $30, $4
sw $1, -4($30)          ; push 1
sub $30, $30, $4
sw $2, -4($30)          ; push 2
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4
lis $5
.word init
jalr $5
add $30, $30, $4         ; pop 31
lw $31, -4($30)

;---- wain body -----

; Initializing local to 5
lis $3
.word 5
sw $3, -4($30)          ; push 3
sub $30, $30, $4
; (Done initializing local to 5)

; Initializing *ab to NULL
add $3, $11, $0
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Initializing *arr to NULL
add $3, $11, $0
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Initializing c to 0
lis $3
.word 0
sw $3, -4($30)          ; push 3
sub $30, $30, $4
; (Done initializing c to 0)

; Initializing *i to NULL
add $3, $11, $0
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Setting i to 'expr'

; Taking the address

; ID c
lis $3
.word -20
add $3, $3, $29
sw $3, -24($29)
; (Done setting i to 'expr')

; Setting arr to 'expr'
lis $3
.word 5
add $1, $3, $0
sw $31, -4($30)          ; push 31
sub $30, $30, $4
lis $5
.word new
jalr $5
add $30, $30, $4         ; pop 31
lw $31, -4($30)
bne $3, $0, 1
add $3, $11, $0
sw $3, -16($29)
; (Done setting arr to 'expr')

; Setting ab to 'expr'
lis $3
.word 23
add $1, $3, $0
sw $31, -4($30)          ; push 31
sub $30, $30, $4
lis $5
.word new
jalr $5
add $30, $30, $4         ; pop 31
lw $31, -4($30)
bne $3, $0, 1
add $3, $11, $0
sw $3, -12($29)
; (Done setting ab to 'expr')

; Setting ab to 'expr'

; Taking the address

; ID b
lis $3
.word -4
add $3, $3, $29
sw $3, -12($29)
; (Done setting ab to 'expr')

; Printing 
sw $1, -4($30)          ; push 1
sub $30, $30, $4

; Calling procedure Fabc (has args)
sw $29, -4($30)          ; push 29
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4

; arg: 
lw $3, 0($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; arg: 
lw $3, -4($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; arg: 
lw $3, -4($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $5
.word Fabc
jalr $5
 ; NUM PARAMS = 3
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 29
lw $29, -4($30)
add $1, $3, $0
sw $31, -4($30)          ; push 31
sub $30, $30, $4
jalr $10                 ; Print 
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 1
lw $1, -4($30)


; If statement
; (Evaluating expr with arithmetic)
 ; evaluating = int PLUS int
lis $3
.word 7
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 3
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 10
add $30, $30, $4         ; pop 5
lw $5, -4($30)
slt $6, $3, $5
slt $7, $5, $3
add $3, $6, $7
beq $3, $0, else0

; If statement
; (Evaluating expr with arithmetic)
 ; evaluating = int PLUS int
; (Evaluating expr with arithmetic)
 ; evaluating = int STAR int
lw $3, -8($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Calling procedure Fbar (no args)
sw $29, -4($30)          ; push 29
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4
lis $5
.word Fbar
jalr $5
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 29
lw $29, -4($30)
; (Done calling procedure Fbar (no args))
add $30, $30, $4         ; pop 5
lw $5, -4($30)
mult $5, $3
mflo $3
; (Done evaluating expr with arithmetic)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 3
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 4
add $30, $30, $4         ; pop 5
lw $5, -4($30)
slt $6, $3, $5
slt $7, $5, $3
add $3, $6, $7
sub $3, $11, $3
beq $3, $0, else1

; While loop
loop0:
lw $3, -8($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 0
add $30, $30, $4         ; pop 5
lw $5, -4($30)
slt $3, $3, $5
beq $3, $0, endWhile0

; Setting local to 'expr'
; (Evaluating expr with arithmetic)
 ; evaluating = int MINUS int
lw $3, -8($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 1
add $30, $30, $4         ; pop 5
lw $5, -4($30)
sub $3, $5, $3
; (Done evaluating expr with arithmetic)
sw $3, -8($29)
; (Done setting local to 'expr')
beq $0, $0, loop0
endWhile0:

; End while loop
beq $0, $0, endif1

; Else statement
else1:
endif1:
beq $0, $0, endif0

; Else statement
else0:

; While loop
loop1:

; Dereferencing
lw $3, -24($29)
lw $3, 0($3)
; (Done dereferencing)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 5
add $30, $30, $4         ; pop 5
lw $5, -4($30)
slt $3, $5, $3
beq $3, $0, endWhile1

; Setting * to 'expr'
; (Evaluating expr with arithmetic)
 ; evaluating = int PLUS int
lis $3
.word 0
sw $3, -4($30)          ; push 3
sub $30, $30, $4
; (Evaluating expr with arithmetic)
 ; evaluating = int STAR int

; Calling procedure Ffoo (has args)
sw $29, -4($30)          ; push 29
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4

; arg: 

; Taking the address

; ID local
lis $3
.word -8
add $3, $3, $29
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $5
.word Ffoo
jalr $5
 ; NUM PARAMS = 1
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 29
lw $29, -4($30)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Dereferencing
lw $3, -24($29)
lw $3, 0($3)
; (Done dereferencing)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
mult $5, $3
mflo $3
; (Done evaluating expr with arithmetic)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
; (Evaluating expr with arithmetic)
 ; evaluating = int* PLUS int
lw $3, -16($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Dereferencing
lw $3, -24($29)
lw $3, 0($3)
; (Done dereferencing)
mult $3, $4
mflo $3
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
sw $5, 0($3)
; (Done setting * to 'expr')

; Setting * to 'expr'
; (Evaluating expr with arithmetic)
 ; evaluating = int PLUS int

; Dereferencing
lw $3, -24($29)
lw $3, 0($3)
; (Done dereferencing)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 1
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lw $3, -24($29)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
sw $5, 0($3)
; (Done setting * to 'expr')
beq $0, $0, loop1
endWhile1:

; End while loop
endif0:

; Returning from main

; Dereferencing
; (Evaluating expr with arithmetic)
 ; evaluating = int* MINUS int
; (Evaluating expr with arithmetic)
 ; evaluating = int* PLUS int
lw $3, -16($29)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Dereferencing
lw $3, -24($29)
lw $3, 0($3)
; (Done dereferencing)
mult $3, $4
mflo $3
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 1
mult $3, $4
mflo $3
add $30, $30, $4         ; pop 5
lw $5, -4($30)
sub $3, $5, $3
; (Done evaluating expr with arithmetic)
lw $3, 0($3)
; (Done dereferencing)

; Restoring stack at end of main
add $30, $30, $4
add $30, $30, $4

;---- wain epilogue -----
add $30, $29, $4
jr $31


;<------------------ Fbar ---------------->
Fbar:

;---- Fbar prologue -----
sub $29, $30, $4

;---- Fbar body -----
sw $1, -4($30)          ; push 1
sub $30, $30, $4
sw $2, -4($30)          ; push 2
sub $30, $30, $4
sw $5, -4($30)          ; push 5
sub $30, $30, $4
sw $29, -4($30)          ; push 29
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4
lis $3
.word 5
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 29
lw $29, -4($30)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 2
lw $2, -4($30)
add $30, $30, $4         ; pop 1
lw $1, -4($30)

; Returning from Fbar

;---- Fbar epilogue -----
add $30, $29, $4
jr $31

;<------------------ Ffoo ---------------->
Ffoo:

;---- Ffoo prologue -----
sub $29, $30, $4

;---- Ffoo body -----

; Initializing *local to NULL
add $3, $11, $0
sw $3, -4($30)          ; push 3
sub $30, $30, $4
sw $1, -4($30)          ; push 1
sub $30, $30, $4
sw $2, -4($30)          ; push 2
sub $30, $30, $4
sw $5, -4($30)          ; push 5
sub $30, $30, $4
sw $29, -4($30)          ; push 29
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4

; Setting local to 'expr'
lw $3, 4($29)
sw $3, 0($29)
; (Done setting local to 'expr')
; (Evaluating expr with arithmetic)
 ; evaluating = int PLUS int
lis $3
.word 62
sw $3, -4($30)          ; push 3
sub $30, $30, $4
; (Evaluating expr with arithmetic)
 ; evaluating = int STAR int

; Dereferencing
lw $3, 0($29)
lw $3, 0($3)
; (Done dereferencing)
sw $3, -4($30)          ; push 3
sub $30, $30, $4

; Dereferencing
lw $3, 4($29)
lw $3, 0($3)
; (Done dereferencing)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
mult $5, $3
mflo $3
; (Done evaluating expr with arithmetic)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $3, $5, $3
; (Done evaluating expr with arithmetic)
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 29
lw $29, -4($30)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 2
lw $2, -4($30)
add $30, $30, $4         ; pop 1
lw $1, -4($30)

; Returning from Ffoo

;---- Ffoo epilogue -----
add $30, $29, $4
jr $31

;<------------------ Fabc ---------------->
Fabc:

;---- Fabc prologue -----
sub $29, $30, $4

;---- Fabc body -----

; Initializing sum to 5848
lis $3
.word 5848
sw $3, -4($30)          ; push 3
sub $30, $30, $4
; (Done initializing sum to 5848)
sw $1, -4($30)          ; push 1
sub $30, $30, $4
sw $2, -4($30)          ; push 2
sub $30, $30, $4
sw $5, -4($30)          ; push 5
sub $30, $30, $4
sw $29, -4($30)          ; push 29
sub $30, $30, $4
sw $31, -4($30)          ; push 31
sub $30, $30, $4

; Printing 
sw $1, -4($30)          ; push 1
sub $30, $30, $4
; (Evaluating expr with arithmetic)
 ; evaluating = int MINUS int
lis $3
.word 0
sw $3, -4($30)          ; push 3
sub $30, $30, $4
lis $3
.word 1
add $30, $30, $4         ; pop 5
lw $5, -4($30)
sub $3, $5, $3
; (Done evaluating expr with arithmetic)
add $1, $3, $0
sw $31, -4($30)          ; push 31
sub $30, $30, $4
jalr $10                 ; Print 
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 1
lw $1, -4($30)


; Printing 
sw $1, -4($30)          ; push 1
sub $30, $30, $4
lw $3, 0($29)
add $1, $3, $0
sw $31, -4($30)          ; push 31
sub $30, $30, $4
jalr $10                 ; Print 
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 1
lw $1, -4($30)

lis $3
.word 10
add $30, $30, $4         ; pop 31
lw $31, -4($30)
add $30, $30, $4         ; pop 29
lw $29, -4($30)
add $30, $30, $4         ; pop 5
lw $5, -4($30)
add $30, $30, $4         ; pop 2
lw $2, -4($30)
add $30, $30, $4         ; pop 1
lw $1, -4($30)

; Returning from Fabc

;---- Fabc epilogue -----
add $30, $29, $4
jr $31
