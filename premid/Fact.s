.data   
    promptMessage:  .asciiz    "Enter a number to find fac: "
    resultMessage:  .asciiz    "\nThe result is : "
    theNumber:      .word       0
    theAnswer:      .word       0

.text   
    .globl main

    main:
        #Read Input
        li      $v0, 4
        la      $a0, promptMessage
        syscall

        li      $v0, 5
        syscall

        sw      $v0, theNumber

        #Call function
        lw      $a0, theNumber
        jal     Fact
        sw      $v0, theAnswer

        #Print
        li      $v0, 4
        la      $a0, resultMessage
        syscall

        li      $v0, 1
        lw      $a0, theAnswer
        syscall

        # End
        li      $v0, 10
        syscall

    .globl Fact
    Fact:
        subu    $sp, $sp, 8
        sw      $ra, 0($sp)
        sw      $s0, 4($sp)

        # Base case
        li      $v0, 1
        beq     $a0, 0, Done

        # Fact(n-1)
        move    $s0, $a0
        sub     $a0, $a0, 1
        jal     Fact

        mul     $v0, $s0, $v0

    Done:
        lw      $ra, 0($sp)
        lw      $s0, 4($sp)
        addu    $sp, $sp, 8
        jr $ra