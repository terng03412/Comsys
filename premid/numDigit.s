.data   
    promptMessage:  .asciiz    "Enter a number : "
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
        jal     numDigit
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

    .globl numDigit
    numDigit:
        addi	$sp, $sp, -8    #save space for 2 words
        sw		$ra, 4($sp)     #save return address
        sw      $a0, 0($sp)     #save argument
        slti    $t0, $a0, 10    #test n<10
        beq		$t0, $zero, La  #else
        addi    $v0, $zero, 1   #return 1
        addi    $sp, $sp, 8     #pop stack 
        jr		$ra				
        
    La:
        div     $a0, $a0, 10    # n/10
        jal		numDigit		# jump to numDigit and save position to $ra
        lw		$a0, 0($sp)		#restore n
        lw      $ra, 4($sp)     #return address
        addi    $sp, $sp, 8     #pop 2 items
        addi    $v0, $v0, 1     # result = 1 + result
        jr      $ra             # return
        
        