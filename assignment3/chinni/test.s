.globl main 
	.text 


main:
addi $sp, $sp, -4
lw $ra, 0($sp)
addi $sp, $sp, -4
sw $fp, 0($sp)
move $fp, $sp
addi $sp, $sp, -484

addi $sp, $sp, -4
addi $t1, $0, 10
sw $t1, 0($sp)
addi $sp, $sp, -4
addi $t1, $fp, -484
lw $t1, 0($t1)
sw $t1, 0($sp)

addi $t2, $fp,-484
lw $t1, 4($sp)
sw $t1, 0($t2)
addi $sp, $sp, 8
addi $sp, $sp, -4
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -484
lw $t1, 0($t1)
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -480
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $0, 5
sw $t1, 0($sp)
lw $t1, 0($sp)
lw $t2, 4($sp)
li $t3, 48
mul $t1, $t1, $t3
add $t2, $t2, $t1
addi $sp, $sp, 8
addi $sp, $sp, -4
lw $t2, 0($sp)
addi $sp, $sp, -4
addi $t1, $0, 3
sw $t1, 0($sp)
lw $t1, 0($sp)
lw $t2, 4($sp)
li $t3, 4
mul $t1, $t1, $t3
add $t2, $t2, $t1
addi $sp, $sp, 8
move $t9, $t2
lw $t2, 0($t2)
addi $sp, $sp, -4
lw $t2, 0($sp)
lw $t1, 4($sp)
sw $t1, 0($t9)
addi $sp, $sp, 8
addi $sp, $sp, -4
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -480
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $0, 5
sw $t1, 0($sp)
lw $t1, 0($sp)
lw $t2, 4($sp)
li $t3, 48
mul $t1, $t1, $t3
add $t2, $t2, $t1
addi $sp, $sp, 8
addi $sp, $sp, -4
lw $t2, 0($sp)
addi $sp, $sp, -4
addi $t1, $0, 3
sw $t1, 0($sp)
lw $t1, 0($sp)
lw $t2, 4($sp)
li $t3, 4
mul $t1, $t1, $t3
add $t2, $t2, $t1
addi $sp, $sp, 8
move $t9, $t2
lw $t2, 0($t2)
addi $sp, $sp, -4
lw $t2, 0($sp)
li $v0, 1
lw $t1, 0($sp)
move $a0, $t1
syscall

addi $sp, $sp, 4




