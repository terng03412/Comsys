.text # text section 
.globl main # call main by SPIM 
main:
li $a0 1
sw $a0, 0($sp)
addi $sp, $sp, -4
li $a0 1
lw $t1, 4($sp)
mul $a0, $a0, $t1
addi $sp, $sp, 4
li $a0 1
lw $t1, 4($sp)
mul $a0, $a0, $t1
addi $sp, $sp, 4
li   $v0, 1
syscall
end:
ori   $v0, $0, 10  # system call 10 for exit
syscall            # we are out of here.
