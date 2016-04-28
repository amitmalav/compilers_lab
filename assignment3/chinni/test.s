.globl main 

		.data
		.text
main:
addi $sp, $sp, -4
sw $ra, 0($sp)
addi $sp, $sp, -4
sw $fp, 0($sp)
move $fp, $sp
addi $sp, $sp, -44

addi $sp, $sp, -4
addi $t1, $0, 10
sw $t1, 0($sp)
addi $sp, $sp, -4
addi $t1, $fp, -44
sw $t1, 0($sp)

lw $t1, 4($sp)
lw $t2, 0($sp)
sw $t1, 0($t2)
addi $sp, $sp, 8
addi $sp, $sp, -4
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -44
lw $t1, 0($t1)
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -40
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $0, 5
sw $t1, 0($sp)
lw $t1, 0($sp)
lw $t2, 4($sp)
li $t3, 4
mul $t1, $t1, $t3
add $t2, $t2, $t1
addi $sp, $sp, 8
addi $sp, $sp, -4
lw $t2, 0($sp)
lw $t1, 4($sp)
lw $t2, 0($sp)
sw $t1, 0($t2)
addi $sp, $sp, 8
addi $sp, $sp, -4
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $fp, -40
sw $t1, 0($sp)

addi $sp, $sp, -4
addi $t1, $0, 5
sw $t1, 0($sp)
lw $t1, 0($sp)
lw $t2, 4($sp)
li $t3, 4
mul $t1, $t1, $t3
add $t2, $t2, $t1
addi $sp, $sp, 8
lw $t2, 0($t2)
addi $sp, $sp, -4
lw $t2, 0($sp)
lw $t1, 0($sp)
move $a0, $t1
li $v0, 1
syscall

addi $sp, $sp, 4

