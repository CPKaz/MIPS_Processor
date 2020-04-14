  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $4, $4, 20000
   sw $4, 0($5)
   sh $4, 4($5)
   sb $4, 8($5)
   lbu $7, 0($5)
   lhu $8, 4($5)
   lw $9, 8($5)
	.end	__start
	.size	__start, .-__start
