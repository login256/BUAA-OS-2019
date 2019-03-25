#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

LEAF(calculator)

next_line:

li s2, 0
li s3, 0
li s4, 0
li s5, 0

loop_begin:
	lui s1, 0xb000
	lb t1, 0x00(s1)
	beq t1, zero, loop_begin
	nop

sb t1, 0x00(s1)
li t2, 81
beq t1, t2, calc_close
nop

li t2, 43
bne t1, t2, if_else_1
nop
	li s5, 1
j if_end_1
nop

if_else_1:

li t2, 45
bne t1, t2, if_else_2
nop
	li s5, -1
j if_end_2
nop

if_else_2:

li t2, 68
bne t1, t2, if_else_3
nop
	li s4, 1
j if_end_3
nop

if_else_3:

li t2, 10
bne t1, t2, if_else_4
nop

	mult s3, s5
	mflo s3
	add s0, s2, s3

	while_1:
	beq s0, zero, while_1_end
	nop
		li t3, 10
		div s0, t3
		mflo s0
		mfhi t3
		addi t3, t3, 48
		sb t3, 0x00(s1)

	while_1_end:
	j next_line;
	nop

j if_end_4
nop

if_else_4:

bne s4, zero, second
nop
	li t3, 10
	mult s2, t3
	mflo s2
	addi t3, t2, -48
	add s2, s2, t2

j if_end_5
nop

second:
	li t3, 10
	mult s3, t3
	mflo s3
	addi t3, t2, -48
	add s3, s3, t2
if_end_5:

if_end_4:
if_end_3:
if_end_2:
if_end_1:

j	loop_begin
nop

calc_close:
sb zero, 0x10(s1)

END(calculator)
