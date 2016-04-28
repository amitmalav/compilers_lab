.globl main 
	.text 



main:
addi $sp, $sp, -4
lw $ra, 0($sp)
addi $sp, $sp, -4
sw $fp, 0($sp)
move $fp, $sp
addi $sp, $sp, -4

addi $sp, $sp, -4
addi $t1, $0, 1
sw $t1, 0($sp)
addi $sp, $sp, -4
addi $t1, $fp, -8
lw $t1, 0($t1)
sw $t1, 0($sp)

addi $t2, $fp,-8
lw $t1, 4($sp)
sw $t1, 0($t2)
addi $sp, $sp, 8
addi $sp, $sp, -4
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -8
lw $t1, 0($t1)
sw $t1, 0($sp)

lw $t1, 0($sp)
addi $t1, $t1, 1
addi $t2, $fp, -8
sw $t1, 0($t2)
addi $sp, $sp, 4
addi $sp, $sp, -4
sw $t1, 0($sp)
addi $sp, $sp, -4
addi $t1, $fp, -8
lw $t1, 0($t1)
sw $t1, 0($sp)

li $v0, 1
lw $t1, 0($sp)
move $a0, $t1
syscall

addi $sp, $sp, 4


