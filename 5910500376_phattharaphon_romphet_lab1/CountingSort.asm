.data # data section

str:    .byte 'c','a','d','l','j','g','a','r','h','t','o','x','A','H','d','g','d','s','J','K','h','Y','E','a','s','d','u','w','B','R','L','s','d','g','H','o','p','t','x','n','a','s','e','u','r','h' #char str[] = "cadljgarhtoxAHdgdsJKhYEasduwBRLsdgHoptxnaseurh";
output: .space 1000 #char output[N];

RANGE:  .word 255   #define RANGE 255
count:  .space 1024 

new_line: .asciiz "\n" 

.text # text section

.globl main # call main by SPIM

main:
    # first loop i=0
    li $t1,0    #set $t1=0

firstloop:
    sll     $t2,$t1,2       # convert i * 4 ,shift left 2
    sw      $0,count($t2)   # count[i] = 0 
    
    addi    $t1,$t1,1       # i++
    slti    $t3,$t1,256     # if(i < RANGE+1)
    bne     $t3,$0,firstloop  # if(true) jump to "firstloop"

    li $t1, 0

secondloop:
    sll     $t2,$t1,2       # convert i*4; shift left 2 
    lb      $t3,str($t1)    # load str[i] to $t3
    sll     $t4,$t3,2       # convert str[i]*4 shift left 2 

    lw      $t5,count($t4)  # load count[str[i]] 
    addi    $t5,$t5,1       # ++count[str[i]]
    sw      $t5,count($t4)  # store count[str[i]]
    
    addi    $t1,$t1,1       # ++i
    bne     $t3,$0,secondloop # if(str[i] != terminal) loop

    li $t1, 1 #(i==0 && i++)
    addi    $t3,$0,1        # $t3 save 1

thirdloop:
    sll     $t2,$t1,2       # convert i*4; shift left 2 
    lw      $t4,count($t2)  # load count[i] to $t4            
    
    sub     $t5,$t1,$t3     # set i-1 save in $t5

    sll     $t6,$t5,2       # convert (i-1)*4
    lw      $t7,count($t6)  # load count[i-1] to $t7

    add     $t4,$t4,$t7     # count[i] += count[i-1]
    sw      $t4,count($t2)  # store $t4 to count[i]
    
    addi    $t1,$t1,1       # i++
    slti    $t4,$t1,256     # if(i <= RANGE) -> if(i< RANGE+1) -> range 255+1
    bne     $t4,$0,thirdloop # if(true) -> loop

    li      $t1, 0          #i=0
    li      $t9, 1          #t9 = 1

fourthloop:
    sll     $t2,$t1,2       # convert i*4; shift left 2 
    lb      $t3,str($t1)    # load str[i] to $t3

    sll     $t7,$t3,2       # convert str[i] by mul by 4 >shift left 2 
    lw      $t4,count($t7)  # $t4 = count[str[i]] 
    
    sub     $t5,$t4,$t9     # $t5 = count[str[i]]-1 
    sb      $t3,output($t5) # output[count[str]-1] = str[i] 

    sw      $t5,count($t7)  # $t4 = --count[str[i]] 

    addi    $t1,$t1,1       # i++
    bne     $t3,$0,fourthloop # if(str[i] != null) loop

    li      $t1, 0          #i=0

fifthloop:
    lb      $t3,str($t1)    # $t3 = str[i]
    lb      $t4,output($t1) # $t4 = output[i] 
    sb      $t4,str($t1)    # str[i] = output[i] 

    addi    $t1,$t1,1       # i++
    bne     $t3,$0,fifthloop # if(str[i] != null) loop

    li      $t1, 1          #i=1
    # addi    $t1,$0,1        # set i to 1

print:
    lb      $t5,output($t1) # load str[i] to $t3

    li      $v0,11
    move    $a0,$t5         # print
    syscall

    addi    $t1,$t1,1       # i++
    bne     $t5,$0,print    # if(str[i] != null) loop

end:
    ori   $v0, $0, 10       # system call 10 for exit
    syscall                 # we are out of here.