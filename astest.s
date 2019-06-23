.data
_prompt: .ascii "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
main:
	li $v0,4
	la $a0,_prompt
	syscall
	jr $ra
