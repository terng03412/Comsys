.data

i: .space 4
A: .space 80

.text

.globl main

main:
    li		$t0, 0		# $t0 = 0  ; i = 0

    li		$t1, 1		# $t1 = 1  ; const 1
    li      $t2, 2      # $t2 = 2  ; const 2
    
    la		$s0, A		    # load address A[0]
    addi    $t5, $s0, 0     # load array address

loop:
    slti    $t3, $t0, 20    # check i < 20 
    beq     $t3, $zero, end

    div     $t0, $t2        # mod 2
    mfhi    $t4             # result of mod

    beq     $t4, $zero, One # go to A[i] = 1

    li      $t7, 0
    sw      $t7, 0($t5)      #A[i] = 0

    li      $v0,1
	lw      $t8,($t5)
	move    $a0,$t8
	syscall

    addi    $t0, $t0, 1     # i++
    addi    $t5, $t5, 4     # go to A[i++]
    j loop

One:
    sw      $t1, 0($t5)     # A[i] = 1
    
    li      $v0,1
	lw      $t8,($t5)
	move    $a0,$t8
	syscall

    addi    $t0, $t0, 1     # i++
    addi    $t5, $t5, 4     # go to A[i++]
    j loop

end:
	li $v0,10
	syscall



    
    
              
    