# Add the numbers in an array

.data

A:    .space 80
i:      .space 4

n:      .word 20

.text

.globl main # call main by SPIM

main:
     li $9,0             #i($9) = 0

loop:
     sll   $10,$9,2     # convert "i" to word offset
     lw    $10,A($10)   # load a[i]
     add   $8,$8,$10    # sum = sum + a[i];
     addi  $9,$9,1      # for (...; ...; i++
     slti  $10,$9,20     # for (...; i<20;
     bne   $10,$0,loop

     sw    $8,sum($0)   # update final sum in memory
     sw    $9,i($0)     # update final i in memory

  # print sum message
li   $v0, 4
la   $a0, AnsA
syscall

    # print value of sum
li   $v0, 1
add   $a0, $zero, $8
syscall

    # print new line
li   $v0, 4
la   $a0, newline
syscall

li $9,0             #i($9) = 0
li $8,0             #sum($8) = 0



end:
     ori   $v0, $0, 10  # system call 10 for exit
     syscall            # we are out of here.