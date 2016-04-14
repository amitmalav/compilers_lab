.globl main 
	.text 		
main:
	move $fp, $sp
	addi $sp, $sp, -12
	addi $sp, $sp, -4
	addi $t1, $fp, -4
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	addi $t1, $0, 2
	sw $t1, 0($sp)
	lw $t1, 0($sp)
	lw $t2, 4($sp)
	sw $t1, 0($t2)
	addi $sp, $sp, 8
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	addi $t1, $fp, -16
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	addi $t1, $fp, -4
	lw $t1, 0($t1)
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	addi $t1, $fp, -12
	lw $t1, 0($t1)
	sw $t1, 0($sp)
	lw $t1, 0($sp)
	lw $t2, 4($sp)
	add $t1, $t1, $t2
	addi $sp, $sp, 8
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	addi $sp, $sp, -4
	addi $t1, $fp, -8
	lw $t1, 0($t1)
	sw $t1, 0($sp)
	lw $t1, 0($sp)
	lw $t2, 4($sp)
	add $t1, $t1, $t2
	addi $sp, $sp, 8
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	lw $t1, 0($sp)
	lw $t2, 4($sp)
	sw $t1, 0($t2)
	addi $sp, $sp, 8
	addi $sp, $sp, -4
	sw $t1, 0($sp)
