	.file	1 "main.c"
	.section .mdebug.abi32
	.previous
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"main.c:\011main is start ...\012\000"
	.align	2
$LC1:
	.ascii	"%d\000"
	.align	2
$LC2:
	.ascii	"main.c\000"
	.align	2
$LC3:
	.ascii	"main is over is error!\000"
	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,16($sp)
	lui	$4,%hi($LC0)
	jal	printf
	addiu	$4,$4,%lo($LC0)

	jal	calculator
	nop

	lui	$4,%hi($LC1)
	addiu	$4,$4,%lo($LC1)
	jal	printf
	li	$5,233			# 0xe9

	jal	mips_init
	nop

	lui	$4,%hi($LC2)
	addiu	$4,$4,%lo($LC2)
	li	$5,21			# 0x15
	lui	$6,%hi($LC3)
	jal	_panic
	addiu	$6,$6,%lo($LC3)

	.set	macro
	.set	reorder
	.end	main
	.ident	"GCC: (GNU) 4.0.0 (DENX ELDK 4.1 4.0.0)"
