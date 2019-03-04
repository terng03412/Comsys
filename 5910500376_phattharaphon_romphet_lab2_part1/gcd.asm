.data # data section

intro: .asciiz "gcd of 1890 and 3315 is "
new_line: .asciiz "\n"

.text 
.globl main


main:
    li  $t3, 1890    # m = 1890
    li  $t4, 3315    # n = 3315

    add  $v0, $0, 0
    li   $v0, 4
    la   $a0, intro  # print 
    syscall

    addi $sp, $sp, -8
    sw   $t3, 0($sp)  #arg1 = m
    sw   $t4, 4($sp)  #arg2 = n
    jal gcd      

    addi $sp, $sp, 8
    move    $a0, $v0
    li      $v0, 1
    syscall

    li      $v0, 4
    la      $a0, new_line       # print new line
    syscall


end:
    ori $v0, $0, 10             # system call 10 for exit
    syscall                     # we are out of here



gcd:
    addi $sp, $sp, -8
    sw $ra, 4($sp)
    sw $fp, 0($sp)
    move $fp, $sp

    lw $t0, 8($fp)  #t3 
    lw $t1, 12($fp) #t4
    beq $t0, $t1, equal

    slt $t4, $t1, $t0 # m > n
    bne	$t4, $0, nequal	# if $t4 != 0  then go to nequal

    sub	$t5, $t1, $t0		#$t5 = $t1 - $t0
    addi $sp, $sp, -8
    sw $t5, 4($sp)  
    sw $t1, 0($sp)  
    jal gcd

    lw      $ra, 4($fp)    # return value        
    lw      $fp, 0($fp)
    addi    $sp, $sp, 8
    jr      $ra            # return address

nequal:
    sub $t6 , $t0, $t1
    addi $sp, $sp, -8
    sw  $t6, 0($sp)
    sw  $t1, 4($sp)
    jal gcd                     # recursion

    lw      $ra, 4($fp)         # returning value    
    lw      $fp, 0($fp)
    addi    $sp, $sp, 8         
    jr      $ra                 # return address

    


equal:
    lw $ra, 4($fp)
    lw $fp, 0($fp)
    addi $sp, $sp, 8
    addi $v0, $t1,0
    jr $ra




