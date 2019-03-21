.text # text section 
.globl main # call main by SPIM 
main:
li $a0 10
sw $a0, 0($sp)
addi $sp, $sp, -4

li $a0 47
sw $a0, 0($sp)
addi $sp, $sp, -4

addi $sp, $sp, 4
lw $t1, 4($sp)
mul $a0, $a0, $t1
sw  $a0, 4($sp)
li $a0 11
sw $a0, 0($sp)
addi $sp, $sp, -4

addi $sp, $sp, 4
lw $t1, 4($sp)
div $a0, $t1
mfhi $a0
sw  $a0, 4($sp)
li   $v0, 1
syscall
end:
ori   $v0, $0, 10  # system call 10 for exit
syscall            # we are out of here.
