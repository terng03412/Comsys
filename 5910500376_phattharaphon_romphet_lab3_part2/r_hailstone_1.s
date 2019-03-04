main:
	addiu $29, $29, -4
	addiu $8, $0, 7
	sw $8, 0($29)
	jal r_hailstone
	addiu $29, $29, 4
	j exit

r_hailstone:
	lw $9, 0($29)		# load n
	addiu $10, $0, 1
	bne $9, $10, r_hailstone_recur_elseif
	lui $2, 0
	jr $31
	
r_hailstone_recur_elseif:
	lw $9, 0($29)		# load n
	andi $10, $9, 1	# check n % 2
	bne $10, $0, r_hailstone_recur_else
	
	addiu $29, $29, -8
	sw $31, 4($29)
	sw $30, 0($29)
	addu $30, $0, $29
	lw $10, 8($30)		# load n
	
	# n % 2 == 0
	sra $10, $10, 1		# n / 2
	
	addiu $29, $29, -4
	sw $10, 0($29)
	# call r_hailstone(n/2);
	jal r_hailstone
	addiu $29, $29, 4
	
	# 1 + r_hailstone(n/2);
	addiu $2, $2, 1
	
	lw $31, 4($30)
	lw $30, 0($30)
	addiu $29, $29, 8
	jr $31
	
r_hailstone_recur_else:
	addiu $29, $29, -8
	sw $31, 4($29)
	sw $30, 0($29)
	addu $30, $0, $29
	lw $10, 8($30)		# load n
	# n * 3
	addu $11, $0, $10
	addu $11, $11, $10
	addu $11, $11, $10
	addiu $11, $11, 0x1	# n + 1
	
	addiu $29, $29, -4
	sw $11, 0($29)
	# call r_hailstone(3*n+1);
	jal r_hailstone
	addiu $29, $29, 4
	
	# 1 + r_hailstone(3*n+1);
	addiu $2, $2, 1
	
	lw $31, 4($30)
	lw $30, 0($30)
	addiu $29, $29, 8
	jr $31
	
exit: