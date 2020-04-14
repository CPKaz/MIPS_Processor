  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
	addi $1, $1, 40
	addi $2, $2, 7
	sw $2, 0($1)
	lw $5, 0($1)
	add $6, $5, $1 
	.end	__start
	.size	__start, .-__start
