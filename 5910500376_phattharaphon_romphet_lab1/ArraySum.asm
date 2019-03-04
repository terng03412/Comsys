# Add the numbers in an array

.data

sum:    .space 4
i:      .space 4

a:      .word 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
valueb:      .word 0x7fffffff, 0x7ffffffe, 0x7ffffffd, 0x7ffffffc, 0x7ffffffb, 0x7ffffffa, 0x7ffffff9, 0x7ffffff8, 0x7ffffff7, 0x7ffffff6

AnsA: .asciiz "Sum a ="
AnsB: .asciiz "Sum b ="
newline: .asciiz "\n"

.text

.globl main # call main by SPIM

main:
     li $9,0             #i($9) = 0
     li $8,0             #sum($8) = 0


loop:
     sll   $10,$9,2     # convert "i" to word offset
     lw    $10,a($10)   # load a[i]
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

loop2:
     sll   $10,$9,2     # convert "i" to word offset
     lw    $10,valueb($10)   # load valueb[i]
     addu   $8,$8,$10    # sum = sum + valueb[i];
     addi  $9,$9,1      # for (...; ...; i++
     slti  $10,$9,10     # for (...; i<10;
     bne   $10,$0,loop2

     sw    $8,sum($0)   # update final sum in memory
     sw    $9,i($0)     # update final i in memory

li   $v0, 4
la   $a0, AnsB
syscall

li   $v0, 1
add   $a0, $zero, $8
syscall

li   $v0, 4
la   $a0, newline
syscall

end:
     ori   $v0, $0, 10  # system call 10 for exit
     syscall            # we are out of here.