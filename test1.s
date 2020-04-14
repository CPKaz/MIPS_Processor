  .set noat
	.text
	.align	2
	.globl	__start
	.ent	__start
	.type	__start, @function
__start:
   addi $1, $0, 10
   addi $2, $0, 1
   sw $1, 0($0)
   beq $1, $2, end1
loop1:
   sub $1, $1, $2
   bne $1, $2, loop1
end1:
   lw $3, 0($0)
   beq $3, $2, end2
loop2:
   add $2, $2, $1
   bne $3, $2, loop2
end2:
	.end	__start
	.size	__start, .-__start
