.text # text section 
.globl main # call main by SPIM 
main:
li $a0 12
sw $a0, 0($sp)
addi $sp, $sp, -4

li $a0 3
sw $a0, 0($sp)
addi $sp, $sp, -4

addi $sp, $sp, 4
lw $t1, 4($sp)
mul $a0, $a0, $t1
sw  $a0, 4($sp)
li $a0 2
sw $a0, 0($sp)
addi $sp, $sp, -4

li $a0 7
sw $a0, 0($sp)
addi $sp, $sp, -4

addi $sp, $sp, 4
lw $t1, 4($sp)
add $a0, $a0, $t1
sw  $a0, 4($sp)
addi $sp, $sp, 4
lw $t1, 4($sp)
div $t1, $a0
mflo $a0
sw  $a0, 4($sp)
li $a0 5
sw $a0, 0($sp)
addi $sp, $sp, -4

li $a0 2
sw $a0, 0($sp)
addi $sp, $sp, -4

addi $sp, $sp, 4
lw $t1, 4($sp)
div $t1, $a0
mfhi $a0
sw  $a0, 4($sp)
addi $sp, $sp, 4
lw $t1, 4($sp)
sub $a0, $t1, $a0
sw  $a0, 4($sp)
li   $v0, 1
syscall
end:
ori   $v0, $0, 10  # system call 10 for exit
syscall            # we are out of here.
