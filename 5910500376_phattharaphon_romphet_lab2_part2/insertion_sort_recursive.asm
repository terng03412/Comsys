.data # data section

N:          .word 12
data:       .word 132470,  324545,  73245,  93245,  80324542,  244,  2,  66,  236,  327,  236,  21544
newLine:    .asciiz "\n"
space:      .asciiz " "

.text # text section

.globl main # call main by SPIM

main:
    addi    $sp, $sp, -8        # prepare stack (-8)

    lw      $t0, N              # $t0 = N
    la      $t1, data           # $t1 = first pointer of data
    
    sw      $t0, 4($sp)         # $sp(-4) = $t0 = N
    sw      $t1, 0($sp)         # $sp(-8) = $t1 = pointer of data
    jal     print_array         # print_array 
    
    # insertion_sort
    jal     insertion_sort      # jump to insertion_sort 

    # print
    li      $v0, 4
    la      $a0, newLine        # print space
    syscall

    lw      $t0, 4($sp)         # restore N
    lw      $t1, 0($sp)         # restore data
    jal     print_array         # jump to print_array function

end:
    ori     $v0,  $0,  10       # system call 10 for exit
    syscall                     # we are out of here.

print_array:
    addi    $sp, $sp, -8        # prepare stack (-16)

    sw      $ra, 4($sp)         # $sp(-12) = $ra
    sw      $fp, 0($sp)         # $sp(-16) = $fp
    move    $fp, $sp            # $fp = $sp

    lw      $s0, 12($fp)        # $s0 = $sp(-4) = N
    lw      $s1, 8($fp)         # $s1 = $fp(-8) = pointer of data
    li      $s2 , 0             # i = 0    


loop_print:
    #sp = -16
    lw      $s3, 0($s1)         # $s3 = pointer of array
    li      $v0, 1              # v0 = 1
    move    $a0, $s3            # print arr[i]
    syscall

    li      $v0, 4
    la      $a0, space          # print space
    syscall

    addi    $s1, $s1, 4         # $s1 = arr[i+1]
    addi    $s2, $s2, 1         # i++  
    slt     $t0, $s2, $s0       
    bne     $t0, $0, loop_print # if (i < N): jump to loop_print

    addi    $sp, $sp, 8         # restore stack (-8)
    jr      $ra                 # jump to return address

insertion_sort:
    #($sp = -8)
    addi    $sp, $sp, -8        # prepare stack (-16)

    sw      $ra, 4($sp)         # store $ra to $sp (-12)
    sw      $fp, 0($sp)         # store $fp to $sp (-16)
    move    $fp, $sp            # $fp = $sp (-16)
    
    lw      $s0, 12($fp)        # $s0 = N (-4)
    lw      $s1, 8($fp)         # $s1 = first address of arr (-8)

    slti    $t0, $s0, 2         
    bne     $t0, $0, return     # if (N <= 1) return

    addi    $sp, $sp, -8        # prepare stack (-24)
    addi    $s3, $s0, -1        # $s3 = N-1 
    
    sw      $s3, 4($sp)         # $sp (-20) = N-1
    sw      $s1, 0($sp)         # $sp (-24) = first pointer
    jal     insertion_sort      # recursive 

    addi    $sp, $sp, 8         # restore (-16)
    lw      $s0, 12($fp)        # $s0 = N (-4)
    lw      $s1, 8($fp)         # $s1 = pointer of array (-8)
    
    lw      $ra, 4($fp)         # load $ra (-12)
    lw      $fp, 0($fp)         # load $fp 
    addi    $sp, $sp, 8         # restore (-8)

    addi    $s5, $s0, -1        # $s5 = N-1
    sll     $s5, $s5, 2         # 
    add     $s6, $s1, $s5       # $s6 = pointer (arr[N-1])
    lw      $s7, 0($s6)         # $s7 = arr[N-1]
    addi    $t1, $s0, -2        # j = n-2

insertion_loop:

    addi    $t0, $t1, 1         # $t0 = j+1
    sll     $t0, $t0, 2         # 
    add     $t0, $s1, $t0       # $t0 = arr[j+1]

    sll     $t2, $t1, 2         # 
    add     $t3, $t2, $s1       # $t3 = arr[j]
    lw      $t4, 0($t3)         # $t4 = arr[j]

    li      $t5, -1             # t5 = -1
    slt     $t6, $t5, $t1       # if (j >= 0)
    slt     $t7, $s7, $t4       # if (arr[j] > last)
    and     $t6, $t5, $t7       # while (j >= 0 && arr[j] > last)
    beq     $t6, $0, return_recursive     # if false: return

    sw      $t4, 0($t0)         # arr[j+1] = arr[j]
    addi    $t1, $t1, -1        # j--
    j       insertion_loop      # jump to insertion_sort Loop

return:
    lw      $ra, 4($fp)         # load $ra 
    lw      $fp, 0($fp)         # load $fp 
    addi    $sp, $sp, 8         # restore (-8)
    jr      $ra                 # jump to return address

return_recursive:
    sw      $s7, 0($t0)         # arr[j+1] = last
    jr      $ra                 # jump to return address

