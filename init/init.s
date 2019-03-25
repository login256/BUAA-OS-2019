	.file	1 "init.c"
	.section .mdebug.abi32
	.previous
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"init.c:\011mips_init() is called\012\000"
	.align	2
$LC1:
	.ascii	"init.c\000"
	.align	2
$LC2:
	.ascii	"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\000"
	.text
	.align	2
	.globl	mips_init
	.ent	mips_init
	.type	mips_init, @function
mips_init:
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

	lui	$4,%hi($LC1)
	addiu	$4,$4,%lo($LC1)
	li	$5,24			# 0x18
	lui	$6,%hi($LC2)
	jal	_panic
	addiu	$6,$6,%lo($LC2)

	.set	macro
	.set	reorder
	.end	mips_init
	.ident	"GCC: (GNU) 4.0.0 (DENX ELDK 4.1 4.0.0)"
