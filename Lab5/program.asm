.text # text section 
.globl main # call main by SPIM 
main:
li $a0 11
sw $a0 ,0($sp)
addi $sp ,$sp, -4
li $a0 726
lw $t1, 4($sp)
add $a0, $t1, $a0
addi $sp, $sp, 4
addi $sp, $sp, 4
li   $v0, 1
syscall
end:
ori   $v0, $0, 10  # system call 10 for exit
syscall            # we are out of here.
