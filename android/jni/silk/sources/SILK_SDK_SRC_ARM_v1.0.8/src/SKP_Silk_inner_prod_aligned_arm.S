/***********************************************************************
Copyright (c) 2006-2011, Skype Limited. All rights reserved. 
Redistribution and use in source and binary forms, with or without 
modification, (subject to the limitations in the disclaimer below) 
are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in the 
documentation and/or other materials provided with the distribution.
- Neither the name of Skype Limited, nor the names of specific 
contributors, may be used to endorse or promote products derived from 
this software without specific prior written permission.
NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED 
BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
CONTRIBUTORS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF 
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/

#if defined(__arm__)

#include "SKP_Silk_AsmPreproc.h"
#if ( EMBEDDED_ARM >= 7 )

len32					.req	r3
len32tmp				.req	lr
ptr1					.req	r2
ptr2					.req	r1
tmp1					.req	r4
tmp2					.req	r5

val_0					.req	d0
val_1					.req	d1
val_2					.req	d2
val_3					.req	d3
sum_tmp1				.req	q2
sum_tmp2				.req	q3
sum_tmp1_lo				.req	d4
sum_tmp1_hi				.req	d5

.globl	SYM(SKP_Silk_inner_prod_aligned)
SYM(SKP_Silk_inner_prod_aligned):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	vpush	{q0-q7}
	add		fp, sp, #164
	mov			len32, r2								//	put length into r3
	mov			ptr1, r0								//  put in1 to r2
	mov			r0, #0								//	put result to r0	
//	USE SL8D, SI4D
2:
	cmp			len32, #24
	and			len32tmp, len32, #0x7
	blt			3f

	vmov.i32	sum_tmp1, #0
	vld1.16		{val_0, val_1}, [ptr2]!
	vld1.16		{val_2, val_3}, [ptr1]!
	vmov.i32	sum_tmp2, #0								// Set Q2, Q3 to 0
	sub			len32, len32, #16
0:	
	subs		len32, len32, #8
	vmlal.s16	sum_tmp1, val_0, val_2
	vmlal.s16	sum_tmp2, val_1, val_3
	vld1.16		{val_0, val_1}, [ptr2]!
	vld1.16		{val_2, val_3}, [ptr1]!
	bge			0b
	
	vmlal.s16	sum_tmp1, val_0, val_2
	vmlal.s16	sum_tmp2, val_1, val_3
	vadd.s32	sum_tmp1, sum_tmp1, sum_tmp2
	vadd.s32	val_0, sum_tmp1_lo, sum_tmp1_hi
	vmov		tmp1, tmp2, val_0
	cmp			len32tmp, #0								// Check if length%4 == 0
	add			r0, r0, tmp1
	add			r0, r0, tmp2
	bgt			1f									// Jump to process the reminder
	
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}
	
sum_tmp3				.req	q1
sum_tmp3_lo				.req	d2
sum_tmp3_hi				.req	d3
//	USE SL4D, SI4D
3:	
	cmp			len32, #12
	and			len32tmp, len32, #0x3
	movlt		len32tmp, len32								//	if length is not enough for SIMD. 
	blt			1f
	
	vld1.16		val_0, [ptr2]!
	vld1.16		val_1, [ptr1]!
	vmov.i32	sum_tmp3, #0 
	sub			len32, len32, #8
0:	
	subs		len32, len32, #4
	vmlal.s16	sum_tmp3, val_0, val_1
	vld1.16		val_0, [ptr2]!
	vld1.16		val_1, [ptr1]!
	bge			0b
	
	vmlal.s16	sum_tmp3, val_0, val_1
	vadd.s32	val_0, sum_tmp3_lo, sum_tmp3_hi
	vmov		tmp1, tmp2, val_0
	cmp			len32tmp, #0								// Check if length%4 == 0
	add			r0, r0, tmp1
	add			r0, r0, tmp2
	bgt			1f									// Jump to process the reminder
	
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

tmp0				.req	r3

1:
	subs		len32tmp, len32tmp, #1
	ldrsh		tmp0, [ptr2], #2
	ldrsh		tmp1, [ptr1], #2
	beq			2f	
0:	
	smlabb		r0, tmp0, tmp1, r0	
	ldrsh		tmp0, [ptr2], #2
	ldrsh		tmp1, [ptr1], #2
	subs		len32tmp, len32tmp, #1
	bgt			0b

2:
	smlabb		r0, tmp0, tmp1, r0
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

len64					.req	r4
len64tmp				.req	lr
ptr00					.req	r2
ptr01					.req	r3
val0					.req	d0
val1					.req	d1
val2					.req	d2
val3					.req	d3
mul0					.req	q2
mul0_lo					.req	d4
mul0_hi					.req	d5
mul1					.req	q3
mul1_lo					.req	d6
mul1_hi					.req	d7
accu0					.req	q4
accu0_lo				.req	d8
accu0_hi				.req	d9
accu1					.req	q5
accu1_lo				.req	d10
accu1_hi				.req	d11
	
.globl	SYM(SKP_Silk_inner_prod16_aligned_64)
SYM(SKP_Silk_inner_prod16_aligned_64):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	vpush	{q0-q7}
	add		fp, sp, #164
	mov			len64, r2
	mov			ptr00, r0
	mov			ptr01, r1
	mov			r0, #0						/*Output*/
	mov			r1, #0

//	USE SL8D, SI4D
2:	
	cmp			len64, #24
	and			len64tmp, len64, #0x7
	blt			3f
	
	vld1.16		{val0, val1}, [ptr00]!
	vld1.16		{val2, val3}, [ptr01]!
	vmov		accu0_lo, r0, r1 
	vmov		accu0_hi, r0, r1
	vmov		accu1, accu0 
	sub			len64, len64, #16
0:	
	vmull.s16	mul0, val0, val2
	vmull.s16	mul1, val1, val3
	vld1.16		{val0, val1}, [ptr00]!
	subs		len64, len64, #8
	vqadd.s32	mul0, mul0, mul1
	vld1.16		{val2, val3}, [ptr01]!
	vaddw.s32	accu0, accu0, mul0_lo
	vaddw.s32	accu1, accu1, mul0_hi
	bge			0b
	
	vmull.s16	mul0, val0, val2
	vmull.s16	mul1, val1, val3
	vqadd.s32	mul0, mul0, mul1
	vaddw.s32	accu0, accu0, mul0_lo
	vaddw.s32	accu1, accu1, mul0_hi
	vqadd.s64	accu0, accu0, accu1
	vqadd.s64	accu0_lo, accu0_lo, accu0_hi
	vmov		r0, r1, accu0_lo
	cmp			len64tmp, #0								// Check if length%4 == 0
	bgt			1f									// Jump to process the reminder
	
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

mul2				.req	q1
mul2_lo				.req	d2
mul2_hi				.req	d3
accu2				.req	q2
accu2_lo			.req	d4
accu2_hi			.req	d5
accu3				.req	q3

//	USE SL4D, SI4D
3:	
	cmp			len64, #12
	and			len64tmp, len64, #0x3
	movlt		len64tmp, len64								//	if length is not enough for SIMD. 
	blt			1f
	
	vld1.16		val0, [ptr00]!
	vld1.16		val1, [ptr01]!
	vmov		accu2_lo, r0, r1 
	vmov		accu2_hi, r0, r1
	vmov		accu3, accu2 
	sub			len64, len64, #8
0:	
	vmull.s16	mul2, val0, val1
	vld1.16		val0, [ptr00]!
	subs		len64, len64, #4
	vaddw.s32	accu2, accu2, mul2_lo
	vld1.16		val1, [ptr01]!
	vaddw.s32	accu3, accu3, mul2_hi
	bge			0b
	
	vmull.s16	mul2, val0, val1
	vaddw.s32	accu2, accu2, mul2_lo
	vaddw.s32	accu3, accu3, mul2_hi
	vqadd.s64	accu2, accu2, accu3
	vqadd.s64	accu2_lo, accu2_lo, accu2_hi
	vmov		r0, r1, accu2_lo
	cmp			len64tmp, #0
	bgt			1f
	
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

val4				.req	r4
val5				.req	r5
1:
	subs		len64tmp, len64tmp, #1
	ldrsh		val4, [ptr00], #2
	ldrsh		val5, [ptr01], #2
	beq			2f	
0:	
	smlalbb		r0, r1, val4, val5
	ldrsh		val4, [ptr00], #2
	ldrsh		val5, [ptr01], #2
	subs		len64tmp, len64tmp, #1
	bgt			0b

2:
	smlalbb		r0, r1, val4, val5
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

#elif EMBEDDED_ARM >=5 

/*
 *	SKP_Silk_inner_prod_aligned(val1_16bit[], val2_16bit[], len)
 *	
 *	Known issue: 
 *		1. val1_16bit and val2_16bit needs to be 16bit aligned. 
 *		2. result is in 32bit, no saturation, wrap around instead.
 */
sum 	.req 	r0
val_p1	.req 	r1
val_p2	.req	r2
len		.req	r3
val1	.req	r4
val2	.req	r5
val3	.req	r6

#ifdef IPHONE
val4	.req	r8
tmp		.req	sb
val5	.req	sl
val6	.req	_r7
val7	.req	lr
val8	.req	ip
#else
val4	.req	_r7
tmp		.req	r8
val5	.req	sb
val6	.req	sl
val7	.req	lr
val8	.req	ip
#endif

.globl	SYM(SKP_Silk_inner_prod_aligned)
SYM(SKP_Silk_inner_prod_aligned):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36

	cmp		r2, #14
	blt		9f/*LenLessThan14*/
	
	ands	tmp, r2, #1						/*check if len is a even number*/
	mov		len, r2
	mov		val_p2, r0
	mov		sum, #0
	beq		0f/*LenEven*/
	
	ldrsh	val3, [val_p1], #2
	ldrsh	val4, [val_p2], #2
	sub		len, len, #1
	smulbb	sum, val3, val4
/*LenEven:*/
0:
	ands	val1, val_p1, #2				/*Check if val_p1 is 4B aligned.*/
	bgt		1f/*R1Odd*/							
	ands	val2, val_p2, #2				/*Check if val_p2 is 4B aligned*/
	bgt		2f/*R2Odd*/
	
/*R1R2Even:*/
	ands	tmp, len, #3
	beq		4f/*Len4*/
	sub		len, len, #2
	ldr		val1, [val_p1], #4
	ldr		val2, [val_p2], #4
	SKP_SMLAD	sum, val1, val2, sum
4:/*Len4:*/
	ands	tmp, len, #7
	beq		8f/*Len8*/
	ldmia	val_p1!, {val1, val3}
	ldmia	val_p2!, {val2, val4}
	sub		len, len, #4
	SKP_SMLAD	sum, val1, val2, sum
	SKP_SMLAD	sum, val3, val4, sum

8:/*Len8:*/
	ldmia	val_p1!, {val1, val3, val5, val7}
	ldmia	val_p2!, {val2, val4, val6, val8}
0:
	subs	len, len, #8	
	SKP_SMLAD	sum, val1, val2, sum
	SKP_SMLAD	sum, val3, val4, sum
	SKP_SMLAD	sum, val5, val6, sum
	SKP_SMLAD	sum, val7, val8, sum
	ldmgtia	val_p1!, {val1, val3, val5, val7}
	ldmgtia	val_p2!, {val2, val4, val6, val8}
	bgt		0b	
	ldmia	sp!,  {r4-r10, fp, ip, pc}

2:/*R2Odd:*/
	ands	tmp, len, #3
	beq		6f/*Len4R2Odd*/
	ldr		val1, [val_p1], #4
	ldrsh	val3, [val_p2], #2
	ldrsh	val4, [val_p2], #2				/*make val_p2 even*/
	sub		len, len, #2
	smlabb	sum, val1, val3, sum
	smlatb	sum, val1, val4, sum
6:/*Len4R2Odd:*/
	sub		len, len, #4
	ldrsh	tmp, [val_p2], #2				/*make val_p2 even*/
	ldmia	val_p1!, {val1, val3}
	ldmia	val_p2!, {val2, val4}
	mov		tmp, tmp, lsl #16
0:
	subs	len, len, #4	
	smlabt	sum, val1, tmp, sum
	smlatb	sum, val1, val2, sum
	smlabt	sum, val3, val2, sum
	smlatb	sum, val3, val4, sum
	mov		tmp, val4
	ldmia	val_p1!, {val1, val3}
	ldmia	val_p2!, {val2, val4}
	bgt		0b	
	smlabt	sum, val1, tmp, sum
	smlatb	sum, val1, val2, sum
	smlabt	sum, val3, val2, sum
	smlatb	sum, val3, val4, sum
	ldmia	sp!,  {r4-r10, fp, ip, pc}
	
1:/*R1Odd:*/
	ands	val2, val_p2, #2				/*Check if val_p2 is 4B aligned*/
	bgt		3f/*R1R2Odd*/
	
	ands	tmp, len, #3
	beq		5f/*Len4R1Odd*/
	ldrsh	val1, [val_p1], #2
	ldrsh	val2, [val_p1], #2
	ldr		val3, [val_p2], #4				/*make val_p2 even*/
	sub		len, len, #2
	smlabb	sum, val1, val3, sum
	smlabt	sum, val2, val3, sum
5:/*Len4R1Odd:*/
	sub		len, len, #4
	ldrsh	tmp, [val_p1], #2				/*make val_p2 even*/
	ldmia	val_p1!, {val1, val3}
	ldmia	val_p2!, {val2, val4}
	mov		tmp, tmp, lsl #16
0:
	subs	len, len, #4	
	smlatb	sum, tmp, val2, sum
	smlabt	sum, val1, val2, sum
	smlatb	sum, val1, val4, sum
	smlabt	sum, val3, val4, sum
	mov		tmp, val3
	ldmia	val_p1!, {val1, val3}
	ldmia	val_p2!, {val2, val4}
	bgt		0b	
	smlatb	sum, tmp, val2, sum
	smlabt	sum, val1, val2, sum
	smlatb	sum, val1, val4, sum
	smlabt	sum, val3, val4, sum
	ldmia	sp!,  {r4-r10, fp, ip, pc}

3:/*R1R2Odd:*/
	sub		len, len, #4
	ldrsh	val3, [val_p1], #2
	ldrsh	val4, [val_p2], #2
	ldr		val1, [val_p1], #4
	ldr		val2, [val_p2], #4
	smlabb	sum, val3, val4, sum
0:
	subs	len, len, #2	
	SKP_SMLAD	sum, val1, val2, sum
	ldr		val1, [val_p1], #4
	ldr		val2, [val_p2], #4
	bgt		0b	
	ldrsh	val3, [val_p1], #2
	ldrsh	val4, [val_p2], #2
	SKP_SMLAD	sum, val1, val2, sum
	smlabb	sum, val3, val4, sum	
	ldmia	sp!,  {r4-r10, fp, ip, pc}
	
9:/*LenLessThan14:*/
	mov		len, r2
	mov		val_p2, r0
	mov		sum, #0
0:
	ldrsh	val1, [val_p1], #2
	ldrsh	val2, [val_p2], #2
	subs	len, len, #1
	smlabb	sum, val1, val2, sum
	bgt		0b
	ldmia	sp!,  {r4-r10, fp, ip, pc}

/*
 *	SKP_Silk_inner_prod16_aligned_64(val1_16bit[], val2_16bit[], len)
 *	
 *	Known issue: 
 *		1. val1_16bit and val2_16bit needs to be 16bit aligned. 
 *		2. result is in 64bit.
 */

// only redefine those registers. 
sumLo 	.req 	r0
sumHi 	.req 	r1


#val_p2	.req	r2
#len	.req	r3
#val1	.req	r4
#val2	.req	r5
#val3	.req	r6
#val4	.req	r8
#tmp	.req	sb

#ifdef IPHONE
val_p3	.req 	sl
val_5	.req	sb
val_6	.req	_r7
val_7	.req	lr
val_8	.req	ip
#else
val_p3	.req 	sb
val_5	.req	r8
val_6	.req	sl
val_7	.req	lr
val_8	.req	ip
#endif

 
.globl	SYM(SKP_Silk_inner_prod16_aligned_64)
SYM(SKP_Silk_inner_prod16_aligned_64):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	
	cmp			r2, #14
	blt			9f/*LenLessThan14_64*/
	
	ands		tmp, r2, #1						/*check if len is a even number*/
	mov			len, r2
	mov			val_p2, r0
	mov			val_p3, r1
	mov			sumLo, #0
	mov			sumHi, #0
	beq			0f/*LenEven64*/
	
	ldrsh		val3, [val_p3], #2
	ldrsh		val4, [val_p2], #2
	sub			len, len, #1
	smlalbb		sumLo, sumHi, val3, val4
0:/*LenEven64:*/
	ands		val1, val_p3, #2				/*Check if val_p3 is 4B aligned.*/
	bgt			1f/*R1Odd64*/							
	ands		val2, val_p2, #2				/*Check if val_p2 is 4B aligned*/
	bgt			2f/*R2Odd64*/
/*R1R2Even64:*/			
	ands		tmp, len, #3
	beq			4f/*Len464*/
	sub			len, len, #2
	ldr			val1, [val_p3], #4
	ldr			val2, [val_p2], #4
	SKP_SMLALD	sumLo, sumHi, val1, val2
	
4:/*Len464:*/	
	ands		tmp, len, #7
	beq			8f/*Len864*/
	sub			len, len, #4
	ldmia		val_p3!, {val1, val3}
	ldmia		val_p2!, {val2, val4}
	SKP_SMLALD	sumLo, sumHi, val1, val2
	SKP_SMLALD	sumLo, sumHi, val3, val4
	
8:/*Len864:*/	
	ldmia		val_p3!, {val1, val3, val_5, val_7}
	ldmia		val_p2!, {val2, val4, val_6, val_8}
0:
	subs		len, len, #8	
	SKP_SMLALD	sumLo, sumHi, val1, val2
	SKP_SMLALD	sumLo, sumHi, val3, val4
	SKP_SMLALD	sumLo, sumHi, val_5, val_6
	SKP_SMLALD	sumLo, sumHi, val_7, val_8
	ldmgtia		val_p3!, {val1, val3, val_5, val_7}
	ldmgtia		val_p2!, {val2, val4, val_6, val_8}
	bgt			0b	
	ldmia	sp!,  {r4-r10, fp, ip, pc}

2:/*R2Odd64:*/
	sub			len, len, #2
	sub			val_p2, val_p2, #2				/*make val_p2 even*/
	ldr			val1, [val_p3], #4
	ldr			val3, [val_p2], #4
	ldr			val2, [val_p2], #4
0:
	subs		len, len, #2	
	smlalbt		sumLo, sumHi, val1, val3
	smlaltb		sumLo, sumHi, val1, val2
	mov			val3, val2
	ldr			val1, [val_p3], #4
	ldr			val2, [val_p2], #4
	bgt			0b	
	smlalbt		sumLo, sumHi, val1, val3
	smlaltb		sumLo, sumHi, val1, val2
	ldmia	sp!,  {r4-r10, fp, ip, pc}

1:/*R1Odd64:*/
	ands		val2, r2, #2					/*Check if val_p2 is 4B aligned*/
	bgt			3f/*R1R2Odd64*/
	sub			len, len, #2
	sub			val_p3, val_p3, #2				/*make val_p3 even*/
	ldr			val3, [val_p3], #4
	ldr			val1, [val_p3], #4
	ldr			val2, [val_p2], #4
0:
	subs		len, len, #2	
	smlaltb		sumLo, sumHi, val3, val2
	smlalbt		sumLo, sumHi, val1, val2
	mov			val3, val1
	ldr			val1, [val_p3], #4
	ldr			val2, [val_p2], #4
	bgt			0b	
	smlaltb		sumLo, sumHi, val3, val2
	smlalbt		sumLo, sumHi, val1, val2
	ldmia	sp!,  {r4-r10, fp, ip, pc}

3:/*R1R2Odd64:*/
	sub			len, len, #4
	ldrsh		val3, [val_p3], #2
	ldrsh		val4, [val_p2], #2
	ldr			val1, [val_p3], #4
	ldr			val2, [val_p2], #4
	smlalbb		sumLo, sumHi, val3, val4
0:
	subs		len, len, #2	
	SKP_SMLALD		sumLo, sumHi, val1, val2
	ldr			val1, [val_p3], #4
	ldr			val2, [val_p2], #4
	bgt			0b	
	ldrsh		val3, [val_p3], #2
	ldrsh		val4, [val_p2], #2
	SKP_SMLALD		sumLo, sumHi, val1, val2
	smlalbb		sumLo, sumHi, val3, val4
	ldmia	sp!,  {r4-r10, fp, ip, pc}
	
9:/*LenLessThan14_64:*/
	mov			len, r2
	mov			val_p2, r0
	mov			val_p3, r1
	mov			sumLo, #0
	mov			sumHi, #0
0:
	ldrsh		val1, [val_p3], #2
	ldrsh		val2, [val_p2], #2
	subs		len, len, #1
	smlalbb		sumLo, sumHi, val1, val2
	bgt			0b
	ldmia	sp!,  {r4-r10, fp, ip, pc}
#endif	
#endif
