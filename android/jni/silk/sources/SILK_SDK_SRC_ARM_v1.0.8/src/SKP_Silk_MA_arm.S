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


#if EMBEDDED_ARM>=7


val_order			.req	r5
val_len				.req	r4
tmp_len				.req	r5

.set	sp_ptr_in, 0 
.set	sp_ptr_B, 4 
.set	sp_ptr_S, 8 
.set	sp_ptr_out, 12 
.globl	SYM(SKP_Silk_MA_Prediction)
SYM(SKP_Silk_MA_Prediction):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	vpush	{q0-q7}
	vpush	{q8-q11}
	add		fp, sp, #228
	sub		sp, sp, #16
.set	arg_len, 248 
.set	arg_order, 252 
/*LOAD INPUT ARGS*/	
	ldr		val_order, [sp, #arg_order]			/*order*/
	ldr		val_len, [sp, #arg_len]			/*len*/
	ands	_r7, r1, #3				/*CHECK: if ( B is 4 byte aligned ) Prerequest for ARMv6 SIMD*/
	bne		2f
	ands	r6, val_order, #1				/*CHECK: if ( order % 2 == 0 ) Prerequest for ARMv6 SIMD*/
	bne		2f
	cmp		val_order, #8					/*CHECK: if ( order == 8 ) ARMv7 SIMD*/
	beq		5f/*SYM(SKP_Silk_MA_Prediction_ARMv7_order8)*/
	
	cmp		val_order, #12					/*CHECK: if ( order == 12 ) ARMv7 SIMD*/
	beq		6f/*SYM(SKP_Silk_MA_Prediction_ARMv7_order12)*/
	
	cmp		val_order, #16					/*CHECK: if ( order == 16 ) ARMv7 SIMD*/
	beq		7f/*SYM(SKP_Silk_MA_Prediction_ARMv7_order16)*/
	cmp		val_order, #6					/*CHECK: if ( order >= 6 ) Prerequest for ARMv6 SIMD*/
	blt		2f
	

ptr1_in				.req	sb
ptr1_out			.req	sl
ptr1_S				.req	ip
ptr1_B				.req	lr
val1_in				.req	r0
val1_B				.req	r6
val1_S0				.req	r1
val1_S1				.req	r2
val1_S2				.req	r3
val1_SO1			.req	_r7
val1_SO2			.req	r8
val1_out			.req	r1
val1_tmp			.req	r3
//  ARMv6 SIMD	
//	order % 2 == 0
	str		r0, [sp, #sp_ptr_in]
	str		r1, [sp, #sp_ptr_B]
	str		r2, [sp, #sp_ptr_S]
	str		r3, [sp, #sp_ptr_out]
	
	mov		ptr1_in, r0					/*in*/
	mov		ptr1_out, r3					/*out*/
	
0:
	ldr		ptr1_S, [sp, #sp_ptr_S]			/*S*/
	ldr		ptr1_B, [sp, #sp_ptr_B]			/*B*/
	ldrsh	val1_in, [ptr1_in], #2			/*in[k]*/
	ldr		val1_S0, [ptr1_S], #4			/*S[0]*/
	ldr		val_order, [sp, #arg_order]			/*order*/
	ldr		val1_S1, [ptr1_S], #4			/*S[1]*/
	rsb		val1_tmp, val1_S0, val1_in, lsl #12		/*SKP_LSHIFT(in16, 12) - S[0]*/
	ldr		val1_B, [ptr1_B], #4			/*B[0], B[1]*/
	
	mov		val1_tmp, val1_tmp, asr #11
	sub		val_order, val_order, #4				/*order - 2 - 2*/
	add		val1_out, r3, #1				/*SKP_RSHIFT_ROUND*/
	ldr		val1_S2, [ptr1_S], #4			/*S[2]*/
	ssat	val1_out, #16, val1_out, asr #1		/*SKP_SAT16( out32 )*/
	strh	val1_out, [ptr1_out], #2			/*save it to out[k]*/
1:
	smlabb	val1_SO1, val1_in, val1_B, val1_S1			/*SKP_SMLABB(S[d + 1], in16, B32)*/
	smlabt	val1_SO2, val1_in, val1_B, val1_S2			/*SKP_SMLABT(S[d + 2], in16, B32)*/
	ldr		val1_S1, [ptr1_S], #4			/*S[d+1]*/
	ldr		val1_S2, [ptr1_S], #-16			/*S[d+2]*/
	ldr		val1_B, [ptr1_B], #4			/*B[d], B[d+1]*/
	subs	val_order, val_order, #2
	str		val1_SO1, [ptr1_S], #4
	str		val1_SO2, [ptr1_S], #16
	bgt		1b
	
	smlabb	val1_SO1, val1_in, val1_B, val1_S1			/*SKP_SMLABB(S[d + 1], in16, B32)*/
	smlabt	val1_SO2, val1_in, val1_B, val1_S2			/*SKP_SMLABT(S[d + 2], in16, B32)*/
	ldr		val1_S1, [ptr1_S], #-12			/*S[d+1]*/
	ldr		val1_B, [ptr1_B]				/*B[d], B[d+1]*/
	str		val1_SO1, [ptr1_S], #4
	str		val1_SO2, [ptr1_S], #4
	
	smlabb	val1_SO1, val1_in, val1_B, val1_S1			/*SKP_SMLABB(S[d + 1], in16, B32)*/
	smulbt	val1_SO2, val1_in, val1_B				/*SKP_SMLABT(S[d + 2], in16, B32)*/	
	subs	val_len, val_len, #1
	str		val1_SO1, [ptr1_S], #4
	str		val1_SO2, [ptr1_S]
	bgt		0b
	
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

ptr2_in				.req	r6
ptr2_out			.req	sb
val2_S0				.req	lr
ptr2_B				.req	_r7
ptr2_S				.req	r8
val2_in				.req	r0
val2_B				.req	r2
val2_B1				.req	r1
val2_S1				.req	r3
val2_out			.req	r1
val2_S2				.req	r2

//	order % 2 != 0 	
2:	
	add		r2, r2, #4
	str		r0, [sp, #sp_ptr_in]
	str		r1, [sp, #sp_ptr_B]
	str		r2, [sp, #sp_ptr_S]
	str		r3, [sp, #sp_ptr_out]
	
	mov		ptr2_in, r0					/*in_ptr*/
	mov		ptr2_out, r3					/*out_ptr*/
	ldr		val2_S0, [r2, #-4]			/*S0*/

0:
	ldrsh	val2_in, [ptr2_in], #2			/*in[k]*/
	ldr		val_order, [sp, #arg_order]			/*order*/
	ldr		ptr2_B, [sp, #sp_ptr_B]
	ldr		ptr2_S, [sp, #sp_ptr_S]			/*S_ptr*/
	rsb		val2_out, val2_S0, val2_in, lsl #12
	ldrsh	val2_in, [ptr2_B], #2
	mov		val2_out, val2_out, asr #11
	ldr		val2_S1, [ptr2_S]
	add		val2_out, val2_out, #1
	smlabb	val2_S0, val2_in, val2_in, val2_S1
	ssat	val2_out, #16, val2_out, asr #1
	sub		val_order, val_order, #3
	ldr		val2_S1, [ptr2_S, #4]
	strh	val2_out, [ptr2_out], #2
	ldrsh	val2_B1, [ptr2_B], #2

1:	
	smlabb	val2_S2, val2_in, val2_B1, val2_S1
	ldr		val2_S1, [ptr2_S, #8]
	ldrsh	val2_B1, [ptr2_B], #2
	str		val2_S2, [ptr2_S], #4
	subs	val_order, val_order, #1
	bgt		1b
	
	smlabb	val2_S2, val2_in, val2_B1, val2_S1
	ldrsh	val2_B1, [ptr2_B], #2
	str		val2_S2, [ptr2_S], #4
	smulbb	val2_S2, val2_in, val2_B1
	subs	val_len, val_len, #1
	str		val2_S2, [ptr2_S]
	bgt		0b
	
	ldr		val2_S2, [sp, #sp_ptr_S]
	str		val2_S0, [r2, #-4]
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}
	
/*SYM(SKP_Silk_MA_Prediction_ARMv7_order8):*/
5:

ptr3_in					.req	sb
ptr3_out				.req	sl
val3_rS0				.req	r6
const3_2048				.req	r3
val3_in0				.req	r0
val3_in1				.req	r1
val3_out32				.req	r8

val3_B0_lo				.req	d0
val3_b0_hi				.req	d1
val3_S0					.req	q2
val3_S0_lo				.req	d4
val3_S0_hi				.req	d5
val3_S1					.req	q3
val3_S1_lo				.req	d6
val3_S1_hi				.req	d7
val3_S2_zero			.req	q4
val3_S_0				.req	q5
const3					.req	q7
val3_in					.req	q1
val3_in_lo				.req	d2
val3_in_hi				.req	d3
val3_out				.req	q6
val3_out_lo				.req	d12
val3_out_hi				.req	d13

	str			r0, [sp, #sp_ptr_in]
	str			r1, [sp, #sp_ptr_B]
	str			r2, [sp, #sp_ptr_S]
	str			r3, [sp, #sp_ptr_out]
	
	cmp			val_len, #4
	mov			ptr3_in, r0
	mov			ptr3_out, r3
	ldr			val3_rS0, [r2]
	vld1.16		{val3_B0_lo, val3_b0_hi}, [r1]								/*read all B*/
	vld1.32		{val3_S0_lo, val3_S0_hi, val3_S1_lo, val3_S1_hi}, [r2]	/*read all S*/
	vmov.i32	val3_S2_zero, #0					/*clear q4*/
	mov			const3_2048, #2048				/*r3 = 1 << 11, will be used for rounding.*/
	and			tmp_len, val_len, #3				/*r5 = r4 % 4 ==> numbers in second loop*/
	blt			3f
	
	vdup.32		const3, const3_2048					/*d12 = [2048] [2048]*/
	sub			val_len, val_len, #4

2:	//	Input/Ouput are processed SI4D
	ldrsh		val3_in0, [ptr3_in], #2			/*in[k]*/
	ldrsh		val3_in1, [ptr3_in], #2
	vext.32		val3_S_0, val3_S_0, val3_S0, #1			/*shift S[2k] in */ 
	
	vdup.16		val3_in, val3_in0					/*mov r0 to q1(d2, d3)*/
	vext.32		val3_S0, val3_S0, val3_S1, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val3_S1, val3_S1, val3_S2_zero, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vmlal.s16	val3_S0, val3_B0_lo, val3_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val3_S1, val3_b0_hi, val3_in_hi				/*calculate S[4-7]*/
	
	vmov		val3_out_lo, val3_in0, val3_in1				/*in[2k], in[2k]+1*/
	vext.32		val3_S_0, val3_S_0, val3_S0, #1			/*shift S[2k] in */ 
	
	vdup.16		val3_in, val3_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val3_S0, val3_S0, val3_S1, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val3_S1, val3_S1, val3_S2_zero, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vmlal.s16	val3_S0, val3_B0_lo, val3_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val3_S1, val3_b0_hi, val3_in_hi				/*calculate S[4-7]*/
	
	ldrsh		val3_in0, [ptr3_in], #2			/*in[k]*/
	ldrsh		val3_in1, [ptr3_in], #2
	vext.32		val3_S_0, val3_S_0, val3_S0, #1			/*shift S[2k] in */ 
	
	vdup.16		val3_in, val3_in0					/*mov r0 to q1(d2, d3)*/
	vext.32		val3_S0, val3_S0, val3_S1, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val3_S1, val3_S1, val3_S2_zero, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vmlal.s16	val3_S0, val3_B0_lo, val3_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val3_S1, val3_b0_hi, val3_in_hi				/*calculate S[4-7]*/
	
	vmov		val3_out_hi, val3_in0, val3_in1				/*in[2k], in[2k]+1*/
	vext.32		val3_S_0, val3_S_0, val3_S0, #1			/*shift S[2k] in */ 
	
	vdup.16		val3_in, val3_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val3_S0, val3_S0, val3_S1, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val3_S1, val3_S1, val3_S2_zero, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vmlal.s16	val3_S0, val3_B0_lo, val3_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val3_S1, val3_b0_hi, val3_in_hi				/*calculate S[4-7]*/
	
	vshl.s32	val3_out, val3_out, #12				/*SKP_LSHIFT(in16, 12)*/
	vsub.s32	val3_out, val3_out, val3_S_0				/*SKP_LSHIFT(in16, 12) - S[0]*/
	vqadd.s32	val3_out, val3_out, const3				/*qadd out32, out32, 0x2048*/
	vqshrn.s32	d10, val3_out, #12
	
	subs		val_len, val_len, #4
	vst1.16		d10, [ptr3_out]!
	bge			2b	
		
	cmp			tmp_len, #0
	beq			4f
	
	vst1.32		val3_S0_lo, [sp]
	ldr			val3_rS0, [sp]				/*r6 = new [S0]*/
3:	//	Input/Ouput are processed 1 by 1
0:
	ldrsh		val3_in0, [ptr3_in], #2			/*in[k]*/
	vext.32		val3_S0, val3_S0, val3_S1, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val3_S1, val3_S1, val3_S2_zero, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vdup.16		val3_in, val3_in0					/*mov r0 to q1(d2, d3)*/
	
	rsb			val3_out32, val3_rS0, r0, lsl #12		/*out32 = SKP_LSHIFT(in16, 12) - S[0];*/
	vmlal.s16	val3_S0, val3_B0_lo, val3_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val3_S1, val3_b0_hi, val3_in_hi				/*calculate S[4-7]*/
	vst1.32		val3_S0_lo, [sp]
	
	qadd		val3_out32, val3_out32, const3_2048
	ssat		val3_out32, #16, val3_out32, asr #12	/*out = round and sat*/
	subs		tmp_len, tmp_len, #1
	strh		val3_out32, [ptr3_out], #2
	ldr			val3_rS0, [sp]				/*r6 = new [S0]*/
	bgt			0b

4:	
	vst1.32		{val3_S0_lo, val3_S0_hi, val3_S1_lo, val3_S1_hi}, [r2]
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}


/*SYM(SKP_Silk_MA_Prediction_ARMv7_order16):*/
7:

ptr4_in					.req	sb
ptr4_out				.req	sl
val4_rS0				.req	r6
const4_2048				.req	r3
val4_in0				.req	r0
val4_in1				.req	r1
val4_out32				.req	r8

val4_B0_lo				.req	d0
val4_B0_hi				.req	d1
val4_B1_lo				.req	d2
val4_B1_hi				.req	d3
val4_S1					.req	q2
val4_S1_lo				.req	d4
val4_S1_hi				.req	d5
val4_S2					.req	q3
val4_S2_lo				.req	d6
val4_S2_hi				.req	d7
val4_S3					.req	q4
val4_S3_lo				.req	d8
val4_S3_hi				.req	d9
val4_S4					.req	q5
val4_S4_lo				.req	d10
val4_S4_hi				.req	d11
val4_S5					.req	q6
val4_S0					.req	q8
val4_S0_lo				.req	d16
val4_in					.req	q9
val4_in_lo				.req	d18
val4_in_hi				.req	d19
val4_out				.req	q10
val4_out_lo				.req	d20
val4_out_hi				.req	d21
val4_const				.req	q7

	str			r0, [sp, #sp_ptr_in]
	str			r1, [sp, #sp_ptr_B]	
	str			r2, [sp, #sp_ptr_S]	
	str			r3, [sp, #sp_ptr_out]
	
	cmp			val_len, #4
	mov			ptr4_in, r0											/*in*/
	mov			ptr4_out, r3										/*out*/
	ldr			val4_rS0, [r2]										/*r6 = S[0]*/
	vld1.16		{val4_B0_lo, val4_B0_hi, val4_B1_lo, val4_B1_hi}, [r1]		/*read all 16 Bs*/
	vld1.32		{val4_S1_lo, val4_S1_hi, val4_S2_lo, val4_S2_hi}, [r2]!		/*read first 16 Ss*/
	vld1.32		{val4_S3_lo, val4_S3_hi, val4_S4_lo, val4_S4_hi}, [r2]	/*read last 16 Ss*/
	vmov.i32	val4_S5, #0						/*clear q6*/
	mov			const4_2048, #2048					/*r3 = 1 << 11, will be used for rounding.*/
	and			tmp_len, val_len, #3					/*r5 = r4 % 4 ==> numbers in second loop*/
	blt			3f
	
	vdup.32		val4_const, const4_2048					/*d12 = [2048] [2048]*/
	sub			val_len, val_len, #4

2:	//	Input/Ouput are processed SI4D
	ldrsh		val4_in0, [ptr4_in], #2			/*in[k]*/
	ldrsh		val4_in1, [ptr4_in], #2
	vext.32		val4_S0, val4_S0, val4_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val4_in, val4_in0					/*mov r0 to q9(d2, d3)*/
	vext.32		val4_S1, val4_S1, val4_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val4_S2, val4_S2, val4_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val4_S3, val4_S3, val4_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vext.32		val4_S4, val4_S4, val4_S5, #1			/*shift q5 by 32bit and put 32-lsb of q6 to 32-msb q5*/
	
	vmlal.s16	val4_S1, val4_B0_lo, val4_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val4_S2, val4_B0_hi, val4_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val4_S3, val4_B1_lo, val4_in_lo				/*calculate S[8-11]*/
	vmlal.s16	val4_S4, val4_B1_hi, val4_in_lo				/*calculate S[12-15]*/
	
	vmov		val4_out_lo, val4_in0, val4_in1				/*in[2k], in[2k]+1  ==> q10*/
	vext.32		val4_S0, val4_S0, val4_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val4_in, val4_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val4_S1, val4_S1, val4_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val4_S2, val4_S2, val4_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val4_S3, val4_S3, val4_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vext.32		val4_S4, val4_S4, val4_S5, #1			/*shift q5 by 32bit and put 32-lsb of q6 to 32-msb q5*/
	
	vmlal.s16	val4_S1, val4_B0_lo, val4_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val4_S2, val4_B0_hi, val4_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val4_S3, val4_B1_lo, val4_in_lo				/*calculate S[8-11]*/
	vmlal.s16	val4_S4, val4_B1_hi, val4_in_lo				/*calculate S[12-15]*/
	
	
	ldrsh		val4_in0, [ptr4_in], #2			/*in[k]*/
	ldrsh		val4_in1, [ptr4_in], #2
	vext.32		val4_S0, val4_S0, val4_S1, #1			/*shift S[2k] in */  
	
	vdup.16		val4_in, val4_in0					/*mov r0 to q9(d2, d3)*/
	vext.32		val4_S1, val4_S1, val4_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val4_S2, val4_S2, val4_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val4_S3, val4_S3, val4_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vext.32		val4_S4, val4_S4, val4_S5, #1			/*shift q5 by 32bit and put 32-lsb of q6 to 32-msb q5*/
	
	vmlal.s16	val4_S1, val4_B0_lo, val4_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val4_S2, val4_B0_hi, val4_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val4_S3, val4_B1_lo, val4_in_lo				/*calculate S[8-11]*/
	vmlal.s16	val4_S4, val4_B1_hi, val4_in_lo				/*calculate S[12-15]*/
	
	vmov		val4_out_hi, val4_in0, val4_in1				/*in[2k], in[2k]+1  ==> q10*/
	vext.32		val4_S0, val4_S0, val4_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val4_in, val4_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val4_S1, val4_S1, val4_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val4_S2, val4_S2, val4_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val4_S3, val4_S3, val4_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vext.32		val4_S4, val4_S4, val4_S5, #1			/*shift q5 by 32bit and put 32-lsb of q6 to 32-msb q5*/
	
	vmlal.s16	val4_S1, val4_B0_lo, val4_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val4_S2, val4_B0_hi, val4_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val4_S3, val4_B1_lo, val4_in_lo				/*calculate S[8-11]*/
	vmlal.s16	val4_S4, val4_B1_hi, val4_in_lo				/*calculate S[12-15]*/
	
	vshl.s32	val4_out, val4_out, #12			/*SKP_LSHIFT(in16, 12)*/
	vsub.s32	val4_out, val4_out, val4_S0			/*SKP_LSHIFT(in16, 12) - S[0]*/
	vqadd.s32	val4_out, val4_out, val4_const			/*qadd out32, out32, 0x2048*/
	vqshrn.s32	val4_S0_lo, val4_out, #12
	
	subs		val_len, val_len, #4
	vst1.16		val4_S0_lo, [ptr4_out]!
	bge			2b	
		
	cmp			tmp_len, #0
	beq			4f
	
	vst1.32		val4_S1_lo, [sp]
	ldr			val4_rS0, [sp]				/*r6 = new [S0]*/
3:	//	Input/Ouput are processed 1 by 1
0:
	ldrsh		val4_in0, [ptr4_in], #2			/*in[k]*/
	vext.32		val4_S1, val4_S1, val4_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val4_S2, val4_S2, val4_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val4_S3, val4_S3, val4_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vext.32		val4_S4, val4_S4, val4_S5, #1			/*shift q5 by 32bit and put 32-lsb of q6 to 32-msb q5*/
	vdup.16		val4_S0, val4_in0					/*mov r0 to q1(d2, d3)*/
	
	rsb			val4_out32, val4_rS0, val4_in0, lsl #12		/*out32 = SKP_LSHIFT(in16, 12) - S[0];*/
	vmlal.s16	val4_S1, val4_B0_lo, val4_S0_lo				/*calculate S[0-3]*/
	vmlal.s16	val4_S2, val4_B0_hi, val4_S0_lo				/*calculate S[4-7]*/
	vmlal.s16	val4_S3, val4_B1_lo, val4_S0_lo				/*calculate S[8-11]*/
	vmlal.s16	val4_S4, val4_B1_hi, val4_S0_lo				/*calculate S[12-15]*/
	vst1.32		val4_S1_lo, [sp]
	
	qadd		val4_out32, val4_out32, const4_2048
	ssat		val4_out32, #16, val4_out32, asr #12	/*out = round and sat*/
	subs		tmp_len, tmp_len, #1
	strh		val4_out32, [ptr4_out], #2
	ldr			val4_rS0, [sp]				/*r6 = new [S0]*/
	bgt			0b

4:	
	sub			r2, r2, #32
	vst1.32		{val4_S1_lo, val4_S1_hi, val4_S2_lo, val4_S2_hi}, [r2]!
	vst1.32		{val4_S3_lo, val4_S3_hi, val4_S4_lo, val4_S4_hi}, [r2]
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}


/*SYM(SKP_Silk_MA_Prediction_ARMv7_order12):*/
6:

ptr5_in					.req	sb
ptr5_out				.req	sl
val5_rS0				.req	r6
val5_in0				.req	r0
val5_in1				.req	r1
const5_2048				.req	r3
val5_out32				.req	r8

val5_B1_lo				.req	d0
val5_B1_hi				.req	d1
val5_B2_lo				.req	d2

val5_S1					.req	q2
val5_S1_lo				.req	d4
val5_S1_hi				.req	d5
val5_S2					.req	q3
val5_S2_lo				.req	d6
val5_S2_hi				.req	d7
val5_S3					.req	q4
val5_S3_lo				.req	d8
val5_S3_hi				.req	d9
val5_S4					.req	q5
val5_S0					.req	q8
val5_S0_lo				.req	d16
val5_const				.req	q7
val5_in					.req	q9
val5_in_lo				.req	d18
val5_out				.req	q10
val5_out_lo				.req	d20
val5_out_hi				.req	d21


	str			r0, [sp, #sp_ptr_in]					/*in*/
	str			r1, [sp, #sp_ptr_B]				/*B*/
	str			r2, [sp, #sp_ptr_S]				/*S*/
	str			r3, [sp, #sp_ptr_out]				/*out*/
	
	cmp			val_len, #4
	mov			ptr5_in, r0						/*in*/
	mov			ptr5_out, r3						/*out*/
	ldr			val5_rS0, [r2]					/*r6 = S[0]*/
	vld1.16		{val5_B1_lo, val5_B1_hi, val5_B2_lo}, [r1]			/*read all 16 Bs*/
	vld1.32		{val5_S1_lo, val5_S1_hi, val5_S2_lo, val5_S2_hi}, [r2]!		/*read first 16 Ss*/
	vld1.32		{val5_S3_lo, val5_S3_hi}, [r2]				/*read last 16 Ss*/
	vmov.i32	val5_S4, #0						/*clear q5*/
	mov			const5_2048, #2048					/*r3 = 1 << 11, will be used for rounding.*/
	and			tmp_len, val_len, #3					/*r5 = r4 % 4 ==> numbers in second loop*/
	blt			3f
	
	vdup.32		val5_const, const5_2048					/*d12 = [2048] [2048]*/
	sub			val_len, val_len, #4

2:	//	Input/Ouput are processed SI4D
	ldrsh		val5_in0, [ptr5_in], #2			/*in[k]*/
	ldrsh		val5_in1, [ptr5_in], #2
	vext.32		val5_S0, val5_S0, val5_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val5_in, val5_in0					/*mov r0 to q9(d2, d3)*/
	vext.32		val5_S1, val5_S1, val5_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val5_S2, val5_S2, val5_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val5_S3, val5_S3, val5_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val5_S1, val5_B1_lo, val5_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val5_S2, val5_B1_hi, val5_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val5_S3, val5_B2_lo, val5_in_lo				/*calculate S[8-11]*/
	
	vmov		val5_out_lo, val5_in0, val5_in1				/*in[2k], in[2k]+1  ==> q10*/
	vext.32		val5_S0, val5_S0, val5_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val5_in, val5_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val5_S1, val5_S1, val5_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val5_S2, val5_S2, val5_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val5_S3, val5_S3, val5_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val5_S1, val5_B1_lo, val5_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val5_S2, val5_B1_hi, val5_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val5_S3, val5_B2_lo, val5_in_lo				/*calculate S[8-11]*/
	
	
	ldrsh		val5_in0, [ptr5_in], #2			/*in[k]*/
	ldrsh		val5_in1, [ptr5_in], #2
	vext.32		val5_S0, val5_S0, val5_S1, #1			/*shift S[2k] in */  
	
	vdup.16		val5_in, val5_in0					/*mov r0 to q9(d2, d3)*/
	vext.32		val5_S1, val5_S1, val5_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val5_S2, val5_S2, val5_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val5_S3, val5_S3, val5_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val5_S1, val5_B1_lo, val5_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val5_S2, val5_B1_hi, val5_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val5_S3, val5_B2_lo, val5_in_lo				/*calculate S[8-11]*/
	
	vmov		val5_out_hi, val5_in0, val5_in1				/*in[2k], in[2k]+1  ==> q10*/
	vext.32		val5_S0, val5_S0, val5_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val5_in, val5_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val5_S1, val5_S1, val5_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val5_S2, val5_S2, val5_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val5_S3, val5_S3, val5_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val5_S1, val5_B1_lo, val5_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val5_S2, val5_B1_hi, val5_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val5_S3, val5_B2_lo, val5_in_lo				/*calculate S[8-11]*/
	
	vshl.s32	val5_out, val5_out, #12			/*SKP_LSHIFT(in16, 12)*/
	vsub.s32	val5_out, val5_out, val5_S0			/*SKP_LSHIFT(in16, 12) - S[0]*/
	vqadd.s32	val5_out, val5_out, val5_const			/*qadd out32, out32, 0x2048*/
	vqshrn.s32	val5_S0_lo, val5_out, #12
	
	subs		val_len, val_len, #4
	vst1.16		val5_S0_lo, [ptr5_out]!
	bge			2b	
		
	cmp			tmp_len, #0
	beq			4f
	
	vst1.32		val5_S1_lo, [sp]
	ldr			val5_rS0, [sp]				/*r6 = new [S0]*/
3:	//	Input/Ouput are processed 1 by 1
0:
	ldrsh		val5_in0, [ptr5_in], #2			/*in[k]*/
	vext.32		val5_S1, val5_S1, val5_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val5_S2, val5_S2, val5_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val5_S3, val5_S3, val5_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vdup.16		val5_S0, val5_in0					/*mov r0 to q8(d16, d17)*/
	
	rsb			val5_out32, val5_rS0, val5_in0, lsl #12		/*out32 = SKP_LSHIFT(in16, 12) - S[0];*/
	vmlal.s16	val5_S1, val5_B1_lo, val5_S0_lo				/*calculate S[0-3]*/
	vmlal.s16	val5_S2, val5_B1_hi, val5_S0_lo				/*calculate S[4-7]*/
	vmlal.s16	val5_S3, val5_B2_lo, val5_S0_lo				/*calculate S[8-11]*/
	vst1.32		val5_S1_lo, [sp]
	
	qadd		val5_out32, val5_out32, const5_2048
	ssat		val5_out32, #16, val5_out32, asr #12	/*out = round and sat*/
	subs		tmp_len, tmp_len, #1
	strh		val5_out32, [ptr5_out], #2
	ldr			val5_rS0, [sp]				/*r6 = new [S0]*/
	bgt			0b

4:	
	sub			r2, r2, #32
	vst1.32		{val5_S1_lo, val5_S1_hi, val5_S2_lo, val5_S2_hi}, [r2]!
	vst1.32		{val5_S3_lo, val5_S3_hi}, [r2]
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}



.set	sp13_ptr_in, 0 
.set	sp13_ptr_B, 4 
.set	sp13_ptr_S, 8 
.set	sp13_ptr_out, 12 
.globl	SYM(SKP_Silk_MA_Prediction_Q13)
SYM(SKP_Silk_MA_Prediction_Q13):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	vpush	{q0-q7}
	vpush	{q8-q11}
	add		fp, sp, #228
	sub		sp, sp, #16
.set	arg13_len, 248 
.set	arg13_order, 252 
/*LOAD INPUT ARGS*/	
val6_order				.req	r5
val6_len				.req	r4
ptr6_in					.req	sb
ptr6_out				.req	sl
ptr6_S					.req	ip
ptr6_B					.req	lr
val6_in					.req	r0
val6_S0					.req	r1
val6_out				.req	r1
val6_S1					.req	r2
val6_S2					.req	r3
val6_B					.req	r6
val6_SO1				.req	_r7
val6_SO2				.req	r8

	ldr		val6_order, [sp, #arg13_order]			/*order*/
	ldr		val6_len, [sp, #arg13_len]			/*len*/
	ands	_r7, r1, #3				/*CHECK: if ( B is 4 byte aligned ) Prerequest for ARMv6 SIMD*/
	bne		2f
	ands	r6, val6_order, #1				/*CHECK: if ( order % 2 == 0 ) Prerequest for ARMv6 SIMD*/
	bne		2f
	
	cmp		val6_order, #12					/*CHECK: if ( order == 12 ) ARMv7 SIMD*/
	beq		8f/*SYM(SKP_Silk_MA_Prediction_Q13_ARMv7_order12)*/
	
	cmp		val6_order, #6					/*CHECK: if ( order >= 6 ) Prerequest for ARMv6 SIMD*/
	blt		2f
	
	
//  ARMv6 SIMD	
//	order % 2 == 0
	str		r0, [sp, #sp13_ptr_in]
	str		r1, [sp, #sp13_ptr_B]
	str		r2, [sp, #sp13_ptr_S]
	str		r3, [sp, #sp13_ptr_out]
	
	mov		ptr6_in, r0					/*in*/
	mov		ptr6_out, r3					/*out*/
	
0:
	ldr		ptr6_S, [sp, #sp13_ptr_S]			/*S*/
	ldr		ptr6_B, [sp, #sp13_ptr_B]			/*B*/
	ldrsh	val6_in, [ptr6_in], #2			/*in[k]*/
	ldr		val6_S0, [ptr6_S], #4			/*S[0]*/
	ldr		val6_order, [sp, #arg13_order]			/*order*/
	ldr		val6_S1, [ptr6_S], #4			/*S[1]*/
	rsb		r3, val6_S0, val6_in, lsl #13		/*SKP_LSHIFT(in16, 12) - S[0]*/
	ldr		val6_B, [ptr6_B], #4			/*B[0], B[1]*/
	
	mov		r3, r3, asr #12
	sub		val6_order, val6_order, #4				/*order - 2 - 2*/
	add		val6_out, r3, #1				/*SKP_RSHIFT_ROUND*/
	ldr		val6_S2, [ptr6_S], #4			/*S[2]*/
	ssat	val6_out, #16, val6_out, asr #1		/*SKP_SAT16( out32 )*/
	strh	val6_out, [ptr6_out], #2			/*save it to out[k]*/
1:
	smlabb	val6_SO1, val6_in, val6_B, val6_S1			/*SKP_SMLABB(S[d + 1], in16, B32)*/
	smlabt	val6_SO2, val6_in, val6_B, val6_S2			/*SKP_SMLABT(S[d + 2], in16, B32)*/
	ldr		val6_S1, [ptr6_S], #4			/*S[d+1]*/
	ldr		val6_S2, [ptr6_S], #-16			/*S[d+2]*/
	ldr		val6_B, [ptr6_B], #4			/*B[d], B[d+1]*/
	subs	val6_order, val6_order, #2
	str		val6_SO1, [ptr6_S], #4
	str		val6_SO2, [ptr6_S], #16
	bgt		1b
	
	smlabb	val6_SO1, val6_in, val6_B, val6_S1			/*SKP_SMLABB(S[d + 1], in16, B32)*/
	smlabt	val6_SO2, val6_in, val6_B, val6_S2			/*SKP_SMLABT(S[d + 2], in16, B32)*/
	ldr		val6_S1, [ptr6_S], #-12			/*S[d+1]*/
	ldr		val6_B, [ptr6_B]				/*B[d], B[d+1]*/
	str		val6_SO1, [ptr6_S], #4
	str		val6_SO2, [ptr6_S], #4
	
	smlabb	val6_SO1, val6_in, val6_B, val6_S1			/*SKP_SMLABB(S[d + 1], in16, B32)*/
	smulbt	val6_SO2, val6_in, val6_B				/*SKP_SMLABT(S[d + 2], in16, B32)*/	
	subs	val6_len, val6_len, #1
	str		val6_SO1, [ptr6_S], #4
	str		val6_SO2, [ptr6_S]
	bgt		0b
	
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}
ptr7_in				.req	r6
ptr7_out			.req	sb
val7_B0				.req	r2
ptr7_B				.req	_r7
ptr7_S				.req	r8
val7_S0				.req	lr
val7_in				.req	r0
val7_out			.req	r1
val7_B				.req	r1
val7_S1				.req	r3
val7_Sout			.req	r2

//	order % 2 != 0 	
2:	
	add		r2, r2, #4
	str		r0, [sp, #sp13_ptr_in]
	str		r1, [sp, #sp13_ptr_B]
	str		r2, [sp, #sp13_ptr_S]
	str		r3, [sp, #sp13_ptr_out]
	
	mov		ptr7_in, r0					/*in_ptr*/
	mov		ptr7_out, r3					/*out_ptr*/
	ldr		val7_S0, [r2, #-4]			/*S0*/

0:
	ldrsh	val7_in, [ptr7_in], #2			/*in[k]*/
	ldr		val6_order, [sp, #arg13_order]			/*order*/
	ldr		ptr7_B, [sp, #sp13_ptr_B]
	ldr		ptr7_S, [sp, #sp13_ptr_S]			/*S_ptr*/
	rsb		val7_out, val7_S0, val7_in, lsl #13
	ldrsh	val7_B0, [ptr7_B], #2
	mov		val7_out, val7_out, asr #12
	ldr		val7_S1, [ptr7_S]
	add		val7_out, val7_out, #1
	smlabb	val7_S0, val7_in, val7_B0, val7_S1
	ssat	val7_out, #16, val7_out, asr #1
	sub		val6_order, val6_order, #3
	ldr		val7_S1, [ptr7_S, #4]
	strh	val7_out, [ptr7_out], #2
	ldrsh	val7_B, [ptr7_B], #2

1:	
	smlabb	val7_Sout, val7_in, val7_B, val7_S1
	ldr		val7_S1, [ptr7_S, #8]
	ldrsh	val7_B, [ptr7_B], #2
	str		val7_Sout, [ptr7_S], #4
	subs	val6_order, val6_order, #1
	bgt		1b
	
	smlabb	val7_Sout, val7_in, val7_B, val7_S1
	ldrsh	val7_B, [ptr7_B], #2
	str		val7_Sout, [ptr7_S], #4
	smulbb	val7_Sout, val7_in, val7_B
	subs	val6_len, val6_len, #1
	str		val7_Sout, [ptr7_S]
	bgt		0b
	
	ldr		r2, [sp, #sp13_ptr_S]
	str		val7_S0, [r2, #-4]
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}
	
/*SYM(SKP_Silk_MA_Prediction_Q13_ARMv7_order12):*/
8:

ptr8_in				.req	sb
ptr8_out			.req	sl
val8_rS0			.req	r6
const8_4096			.req	r3
val8_in0			.req	r0
val8_in1			.req	r1
val8_len			.req	r5
val8_out32			.req	r8

val8_B0_lo			.req	d0
val8_B0_hi			.req	d1
val8_B1_lo			.req	d2

val8_S1				.req	q2
val8_S1_lo			.req	d4
val8_S1_hi			.req	d5
val8_S2				.req	q3
val8_S2_lo			.req	d6
val8_S2_hi			.req	d7
val8_S3				.req	q4
val8_S3_lo			.req	d8
val8_S3_hi			.req	d9
val8_S4				.req	q5
const8				.req	q7
val8_S0				.req	q8
val8_S0_lo			.req	d16
val8_in				.req	q9
val8_in_lo			.req	d18
val8_out			.req	q10
val8_out_lo			.req	d20
val8_out_hi			.req	d21

	str			r0, [sp, #sp13_ptr_in]					/*in*/
	str			r1, [sp, #sp13_ptr_B]				/*B*/
	str			r2, [sp, #sp13_ptr_S]				/*S*/
	str			r3, [sp, #sp13_ptr_out]				/*out*/
	
	cmp			val6_len, #4
	mov			ptr8_in, r0						/*in*/
	mov			ptr8_out, r3						/*out*/
	ldr			val8_rS0, [r2]					/*r6 = S[0]*/
	vld1.16		{val8_B0_lo, val8_B0_hi, val8_B1_lo}, [r1]			/*read all 16 Bs*/
	vld1.32		{val8_S1_lo, val8_S1_hi, val8_S2_lo, val8_S2_hi}, [r2]!		/*read first 16 Ss*/
	vld1.32		{val8_S3_lo, val8_S3_hi}, [r2]				/*read last 16 Ss*/
	vmov.i32	val8_S4, #0						/*clear q5*/
	mov			const8_4096, #4096					/*r3 = 1 << 12, will be used for rounding.*/
	and			val8_len, val6_len, #3					/*r5 = r4 % 4 ==> numbers in second loop*/
	blt			3f
	
	vdup.32		const8, const8_4096					/*d12 = [2048] [2048]*/
	sub			val6_len, val6_len, #4

2:	//	Input/Ouput are processed SI4D
	ldrsh		val8_in0, [ptr8_in], #2			/*in[k]*/
	ldrsh		val8_in1, [ptr8_in], #2
	vext.32		val8_S0, val8_S0, val8_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val8_in, val8_in0					/*mov r0 to q9(d2, d3)*/
	vext.32		val8_S1, val8_S1, val8_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val8_S2, val8_S2, val8_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val8_S3, val8_S3, val8_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val8_S1, val8_B0_lo, val8_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val8_S2, val8_B0_hi, val8_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val8_S3, val8_B1_lo, val8_in_lo				/*calculate S[8-11]*/
	
	vmov		val8_out_lo, val8_in0, val8_in1				/*in[2k], in[2k]+1  ==> q10*/
	vext.32		val8_S0, val8_S0, val8_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val8_in, val8_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val8_S1, val8_S1, val8_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val8_S2, val8_S2, val8_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val8_S3, val8_S3, val8_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val8_S1, val8_B0_lo, val8_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val8_S2, val8_B0_hi, val8_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val8_S3, val8_B1_lo, val8_in_lo				/*calculate S[8-11]*/
	
	
	ldrsh		val8_in0, [ptr8_in], #2			/*in[k]*/
	ldrsh		val8_in1, [ptr8_in], #2
	vext.32		val8_S0, val8_S0, val8_S1, #1			/*shift S[2k] in */  
	
	vdup.16		val8_in, val8_in0					/*mov r0 to q9(d2, d3)*/
	vext.32		val8_S1, val8_S1, val8_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val8_S2, val8_S2, val8_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val8_S3, val8_S3, val8_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val8_S1, val8_B0_lo, val8_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val8_S2, val8_B0_hi, val8_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val8_S3, val8_B1_lo, val8_in_lo				/*calculate S[8-11]*/
	
	vmov		val8_out_hi, val8_in0, val8_in1				/*in[2k], in[2k]+1  ==> q10*/
	vext.32		val8_S0, val8_S0, val8_S1, #1			/*shift S[2k] in */ 
	
	vdup.16		val8_in, val8_in1					/*mov r0 to q1(d2, d3)*/
	vext.32		val8_S1, val8_S1, val8_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val8_S2, val8_S2, val8_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val8_S3, val8_S3, val8_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	
	vmlal.s16	val8_S1, val8_B0_lo, val8_in_lo				/*calculate S[0-3]*/
	vmlal.s16	val8_S2, val8_B0_hi, val8_in_lo				/*calculate S[4-7]*/
	vmlal.s16	val8_S3, val8_B1_lo, val8_in_lo				/*calculate S[8-11]*/
	
	vshl.s32	val8_out, val8_out, #13			/*SKP_LSHIFT(in16, 12)*/
	vsub.s32	val8_out, val8_out, val8_S0			/*SKP_LSHIFT(in16, 12) - S[0]*/
	vqadd.s32	val8_out, val8_out, const8			/*qadd out32, out32, 0x2048*/
	vqshrn.s32	val8_S0_lo, val8_out, #13
	
	subs		val6_len, val6_len, #4
	vst1.16		val8_S0_lo, [ptr8_out]!
	bge			2b	
		
	cmp			val8_len, #0
	beq			4f
	
	vst1.32		val8_S1_lo, [sp]
	ldr			val8_rS0, [sp]				/*r6 = new [S0]*/
3:	//	Input/Ouput are processed 1 by 1
0:
	ldrsh		val8_in0, [ptr8_in], #2			/*in[k]*/
	vext.32		val8_S1, val8_S1, val8_S2, #1			/*shift q2 by 32bit and put 32-lsb of q3 to 32-msb q2*/
	vext.32		val8_S2, val8_S2, val8_S3, #1			/*shift q3 by 32bit and put 32-lsb of q4 to 32-msb q3*/
	vext.32		val8_S3, val8_S3, val8_S4, #1			/*shift q4 by 32bit and put 32-lsb of q5 to 32-msb q4*/
	vdup.16		val8_S0, val8_in0					/*mov r0 to q8(d16, d17)*/
	
	rsb			val8_out32, val8_rS0, val8_in0, lsl #13		/*out32 = SKP_LSHIFT(in16, 12) - S[0];*/
	vmlal.s16	val8_S1, val8_B0_lo, val8_S0_lo				/*calculate S[0-3]*/
	vmlal.s16	val8_S2, val8_B0_hi, val8_S0_lo				/*calculate S[4-7]*/
	vmlal.s16	val8_S3, val8_B1_lo, val8_S0_lo				/*calculate S[8-11]*/
	vst1.32		val8_S1_lo, [sp]
	
	qadd		val8_out32, val8_out32, const8_4096
	ssat		val8_out32, #16, val8_out32, asr #13	/*out = round and sat*/
	subs		val8_len, val8_len, #1
	strh		val8_out32, [ptr8_out], #2
	ldr			val8_rS0, [sp]				/*r6 = new [S0]*/
	bgt			0b

4:	
	sub			r2, r2, #32
	vst1.32		{val8_S1_lo, val8_S1_hi, val8_S2_lo, val8_S2_hi}, [r2]!
	vst1.32		{val8_S3_lo, val8_S3_hi}, [r2]
	add		sp, sp, #16
	vpop	{q8-q11}
	vpop	{q0-q7}
	ldmia	sp!,  {r4-r10, fp, ip, pc}

	
#elif EMBEDDED_ARM>=5
/*
 * void SKP_Silk_MA_Prediction(
 *	const SKP_int16		*in,			 I:	input signal								
 *  const SKP_int16		*B,				 I:	MA coefficients, Q13 [order+1]				
 *	SKP_int32			*S,				 I/O: state vector [order]						
 *	SKP_int16			*out,			 O:	output signal								
 *	const SKP_int32		len,			 I:	signal length								
 *	const SKP_int32		order			 I:	filter order								
 *	)
 *
 *
 */
 
 
ma_in		.req	sb
ma_out		.req	sl
ma_s		.req	ip
ma_b		.req	lr
ma_len		.req	r4

ma_in_val	.req	r0
ma_s0_val	.req	r1
ma_s1_val	.req	r2
ma_order	.req	r5
ma_b_val	.req	r6

ma_tmp0		.req	r3
ma_out_val	.req	r1
ma_const0	.req	r3

ma_s2_val	.req	r3

#ifdef IPHONE
ma_new_s1	.req	r8
ma_new_s2	.req	_r7
#else
ma_new_s1	.req	_r7
ma_new_s2	.req	r8
#endif

 
.set	SAVE_IN, 0 
.set	SAVE_B, 4 
.set	SAVE_S, 8 
.set	SAVE_OUT, 12 

.globl	SYM(SKP_Silk_MA_Prediction)
SYM(SKP_Silk_MA_Prediction):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #16
.set	arg_len, 56 
.set	arg_order, 60 

/*LOAD INPUT ARGS*/	
	ldr		ma_order, [sp, #arg_order]
	ldr		ma_len, [sp, #arg_len]
	ands	_r7, r1, #3
	bne		9f/*MA_Prediction_ORDER_2BYTE*/
	ands	r6, ma_order, #1
	bne		9f/*MA_Prediction_ORDER_2BYTE*/
	cmp		ma_order, #6					
	blt		9f/*MA_Prediction_ORDER_2BYTE*/
//	order % 2 == 0
	str		r0, [sp, #SAVE_IN]
	str		r1, [sp, #SAVE_B]
	str		r2, [sp, #SAVE_S]
	str		r3, [sp, #SAVE_OUT]
	
	mov		ma_in, r0
	mov		ma_out, r3

0:
	ldr		ma_s, [sp, #SAVE_S]
	ldr		ma_b, [sp, #SAVE_B]
	ldrsh	ma_in_val, [ma_in], #2
	ldr		ma_s0_val, [ma_s], #4
	ldr		ma_order, [sp, #arg_order]
	ldr		ma_s1_val, [ma_s], #4
	rsb		ma_tmp0, ma_s0_val, ma_in_val, lsl #12
	ldr		ma_b_val, [ma_b], #4
	qadd	ma_out_val, ma_tmp0, ma_tmp0
	sub		ma_order, ma_order, #4
	mov		ma_const0, #32768
	qadd	ma_out_val, ma_out_val, ma_out_val
	qadd	ma_out_val, ma_out_val, ma_out_val
	qadd	ma_out_val, ma_out_val, ma_out_val
	qadd	ma_out_val, ma_out_val, ma_const0
	ldr		ma_s2_val, [ma_s], #4
	mov		ma_out_val, ma_out_val, asr #16
	strh	ma_out_val, [ma_out], #2
	sub		r1, ma_s, #12	
1:
	smlabb	ma_new_s1, ma_in_val, ma_b_val, ma_s1_val
	smlabt	ma_new_s2, ma_in_val, ma_b_val, ma_s2_val
	ldmia	ma_s!, {ma_s1_val, ma_s2_val}
	ldr		ma_b_val,  [ma_b],  #4
	subs	ma_order, ma_order, #2
	stmia	r1!, {ma_new_s1, ma_new_s2}
	bgt		1b
	smlabb	ma_new_s1, ma_in_val, ma_b_val, ma_s1_val
	smlabt	ma_new_s2, ma_in_val, ma_b_val, ma_s2_val
	ldr		ma_s1_val, [ma_s], #-12
	ldr		ma_b_val,  [ma_b]
	str		ma_new_s1, [ma_s], #4
	str		ma_new_s2, [ma_s], #4
	
	smlabb	ma_new_s1, ma_in_val, ma_b_val, ma_s1_val
	smulbt	ma_new_s2, ma_in_val, ma_b_val
	subs	ma_len, ma_len, #1
	str		ma_new_s1, [ma_s], #4
	str		ma_new_s2, [ma_s]
	bgt		0b
	
	add		sp, sp, #16
	ldmia	sp!,  {r4-r10, fp, ip, pc}

ma1_in		.req	r6
ma1_out		.req	sb
ma1_s0_val	.req	lr
ma1_b		.req	_r7
ma1_s		.req	r8
ma1_b_val	.req	r2
ma1_s1_val	.req	r3
ma1_b0_val	.req	r1
ma1_s2_val	.req	r2
	
//	order % 2 != 0 	
/*MA_Prediction_ORDER_2BYTE:	*/
9:
	add		r2, r2, #4
	str		r0, [sp, #SAVE_IN]
	str		r1, [sp, #SAVE_B]
	str		r2, [sp, #SAVE_S]
	str		r3, [sp, #SAVE_OUT]
	
	mov		ma1_in, r0
	mov		ma1_out, r3
	ldr		ma1_s0_val, [r2, #-4]

0:
	ldrsh	ma_in_val, [ma1_in], #2
	ldr		ma_order, [sp, #arg_order]
	ldr		ma1_b, [sp, #SAVE_B]
	ldr		ma1_s, [sp, #SAVE_S]
	rsb		ma_out_val, ma1_s0_val, ma_in_val, lsl #12
	ldrsh	ma1_b_val, [ma1_b], #2
	qadd	ma_out_val, ma_out_val, ma_out_val
	ldr		ma1_s1_val, [ma1_s]
	qadd	ma_out_val, ma_out_val, ma_out_val
	smlabb	ma1_s0_val, ma_in_val, ma1_b_val, ma1_s1_val
	mov		ma1_s1_val, #32768
	qadd	ma_out_val, ma_out_val, ma_out_val
	qadd	ma_out_val, ma_out_val, ma_out_val
	qadd	ma_out_val, ma_out_val, ma1_s1_val
	mov		ma_out_val, ma_out_val, asr #16
	sub		ma_order, ma_order, #3
	ldr		ma1_s1_val, [ma1_s, #4]
	strh	ma_out_val, [ma1_out], #2
	ldrsh	ma1_b0_val,  [ma1_b], #2

1:	
	smlabb	ma1_s2_val, ma_in_val, ma1_b0_val, ma1_s1_val
	ldr		ma1_s1_val, [ma1_s, #8]
	ldrsh	ma1_b0_val,  [ma1_b], #2
	str		ma1_s2_val, [ma1_s], #4
	subs	ma_order, ma_order, #1
	bgt		1b
	
	smlabb	ma1_s2_val, ma_in_val, ma1_b0_val, ma1_s1_val
	ldrsh	ma1_b0_val,  [ma1_b], #2
	str		ma1_s2_val, [ma1_s], #4
	smulbb	ma1_s2_val, ma_in_val, ma1_b0_val
	subs	ma_len, ma_len, #1
	str		ma1_s2_val, [ma1_s]
	bgt		0b
	
	ldr		r2, [sp, #SAVE_S]
	str		ma1_s0_val, [r2, #-4]
	add		sp, sp, #16
	ldmia	sp!,  {r4-r10, fp, ip, pc}


/*
 * void SKP_Silk_MA_Prediction_Q13(
 *	const SKP_int16		*in,			 I:	input signal								
 *  const SKP_int16		*B,				 I:	MA coefficients, Q13 [order+1]				
 *	SKP_int32			*S,				 I/O: state vector [order]						
 *	SKP_int16			*out,			 O:	output signal								
 *	const SKP_int32		len,			 I:	signal length								
 *	const SKP_int32		order			 I:	filter order								
 *	)
 *
 *
 */

maq13_s			.req	ip 
maq13_b			.req	lr 
maq13_in		.req	sb
maq13_out		.req	sl
maq13_in_val	.req	r0
maq13_out_val	.req	r1
maq13_order		.req	r5
maq13_len		.req	r4
maq13_s1		.req	r1
maq13_s2		.req	r2
maq13_s3		.req	r3
maq13_b1		.req	r6
maq13_s1_new	.req	_r7
maq13_s2_new	.req	r8
 
.set	SAVE_IN, 0 
.set	SAVE_B, 4 
.set	SAVE_S, 8 
.set	SAVE_OUT, 12 
.globl	SYM(SKP_Silk_MA_Prediction_Q13)
SYM(SKP_Silk_MA_Prediction_Q13):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #16
.set	arg_len, 56 
.set	arg_order, 60 
	
/*LOAD INPUT ARGS*/	
	ldr		maq13_order, [sp, #arg_order]
	ldr		maq13_len, [sp, #arg_len]
	ands	_r7, r1, #3
	bne		2f/*MA_Prediction_Q13_ORDER_2BYTE*/
	ands	r6, r5, #1
	bne		2f/*MA_Prediction_Q13_ORDER_2BYTE*/
	cmp		r5, #6					
	blt		2f/*MA_Prediction_Q13_ORDER_2BYTE*/
//	order % 2 == 0
	str		r0, [sp, #SAVE_IN]
	str		r1, [sp, #SAVE_B]
	str		r2, [sp, #SAVE_S]
	str		r3, [sp, #SAVE_OUT]
	
	mov		maq13_in, r0
	mov		maq13_out, r3
	
0:
	ldr		maq13_s, [sp, #SAVE_S]
	ldr		maq13_b, [sp, #SAVE_B]
	ldrsh	maq13_in_val, [maq13_in], #2
	ldr		maq13_s1, [maq13_s], #4
	ldr		maq13_order, [sp, #arg_order]
	ldr		maq13_s2, [maq13_s], #4
	rsb		maq13_s3, maq13_s1, maq13_in_val, lsl #13
	ldr		maq13_b1, [maq13_b], #4
	
	qadd	maq13_out_val, maq13_s3, maq13_s3
	sub		maq13_order, maq13_order, #4
	mov		r3, #32768
	qadd	maq13_out_val, maq13_out_val, maq13_out_val
	qadd	maq13_out_val, maq13_out_val, maq13_out_val
	qadd	maq13_out_val, maq13_out_val, r3
	ldr		maq13_s3, [maq13_s], #4
	mov		maq13_out_val, maq13_out_val, asr #16

	strh	maq13_out_val, [maq13_out], #2
1:
	smlabb	maq13_s1_new, maq13_in_val, maq13_b1, maq13_s2
	smlabt	maq13_s2_new, maq13_in_val, maq13_b1, maq13_s3
	ldr		maq13_s2, [maq13_s], #4
	ldr		maq13_s3, [maq13_s], #-16
	ldr		maq13_b1, [maq13_b], #4
	subs	maq13_order, maq13_order, #2
	str		maq13_s1_new, [maq13_s], #4
	str		maq13_s2_new, [maq13_s], #16
	bgt		1b
	
	smlabb	maq13_s1_new, maq13_in_val, maq13_b1, maq13_s2
	smlabt	maq13_s2_new, maq13_in_val, maq13_b1, maq13_s3
	ldr		maq13_s2, [maq13_s], #-12
	ldr		maq13_b1, [maq13_b]
	str		maq13_s1_new, [maq13_s], #4
	str		maq13_s2_new, [maq13_s], #4
	
	smlabb	maq13_s1_new, maq13_in_val, maq13_b1, maq13_s2
	smulbt	maq13_s2_new, maq13_in_val, maq13_b1
	subs	maq13_len, maq13_len, #1
	str		maq13_s1_new, [maq13_s], #4
	str		maq13_s2_new, [maq13_s]
	bgt		0b
	
	add		sp, sp, #16
	ldmia	sp!,  {r4-r10, fp, ip, pc}


ma1q13_s		.req	r8 
ma1q13_b		.req	_r7
ma1q13_in		.req	r6
ma1q13_out		.req	sb
ma1q13_s1		.req	r3
ma1q13_s0		.req	r2
ma1q13_b1		.req	r1
ma1q13_b_tmp	.req	r2

//	order % 2 != 0 	
/*MA_Prediction_Q13_ORDER_2BYTE:	*/
2:
	add		r2, r2, #4
	str		r0, [sp, #SAVE_IN]
	str		r1, [sp, #SAVE_B]
	str		r2, [sp, #SAVE_S]
	str		r3, [sp, #SAVE_OUT]
	
	mov		ma1q13_in, r0
	mov		ma1q13_out, r3
	ldr		lr, [r2, #-4]

0:
	ldrsh	maq13_in_val, [ma1q13_in], #2
	ldr		maq13_order, [sp, #arg_order]
	ldr		ma1q13_b, [sp, #SAVE_B]
	ldr		ma1q13_s, [sp, #SAVE_S]	
	rsb		maq13_out_val, lr, maq13_in_val, lsl #13
	ldrsh	ma1q13_b_tmp, [ma1q13_b], #2
	qadd	maq13_out_val, maq13_out_val, maq13_out_val
	ldr		ma1q13_s1, [ma1q13_s]
	qadd	maq13_out_val, maq13_out_val, maq13_out_val
	smlabb	lr, maq13_in_val, ma1q13_b_tmp, ma1q13_s1
	mov		r3, #32768
	qadd	maq13_out_val, maq13_out_val, maq13_out_val
	qadd	maq13_out_val, maq13_out_val, r3
	mov		maq13_out_val, maq13_out_val, asr #16
	sub		maq13_order, maq13_order, #3
	ldr		ma1q13_s1, [ma1q13_s, #4]
	strh	maq13_out_val, [sb], #2
	ldrsh	ma1q13_b1, [ma1q13_b], #2

1:	
	smlabb	ma1q13_s0, maq13_in_val, ma1q13_b1, ma1q13_s1
	ldr		ma1q13_s1, [ma1q13_s, #8]
	ldrsh	ma1q13_b1, [ma1q13_b], #2
	str		ma1q13_s0, [ma1q13_s], #4
	subs	maq13_order, maq13_order, #1
	bgt		1b
	
	smlabb	ma1q13_s0, maq13_in_val, ma1q13_b1, ma1q13_s1
	ldrsh	ma1q13_b1, [ma1q13_b], #2
	str		ma1q13_s0, [ma1q13_s], #4
	smulbb	ma1q13_s0, maq13_in_val, ma1q13_b1
	subs	maq13_len, maq13_len, #1
	str		ma1q13_s0, [ma1q13_s]
	bgt		0b
	
	ldr		r2, [sp, #SAVE_S]
	str		lr, [r2, #-4]
	
	add		sp, sp, #16
	ldmia	sp!,  {r4-r10, fp, ip, pc}

#endif

#if EMBEDDED_ARM>=5

#define	SKP_Silk_MAX_ORDER_LPC 16
.set	sp_B, 0 
.set	sp_pin, 32 
.set	sp_pB, 36 
.set	sp_pS, 40 
.set	sp_pout, 44 
.globl	SYM(SKP_Silk_LPC_analysis_filter)
SYM(SKP_Silk_LPC_analysis_filter):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #48
.set	arg_len, 88 
.set	arg_Order, 92 
len			.req	r4
order		.req	r5
S_val		.req	r6
B_val		.req	_r7
S_tmp1		.req	r8
S_tmp2		.req	sb
out32		.req	sl
// Registers not used in main inner loop
in			.req	r6
in_Q12		.req	_r7
B_tmp		.req	r6
pB_tmp		.req	_r7
	str		r0, [sp, #sp_pin]
	str		r1, [sp, #sp_pB]
	str		r2, [sp, #sp_pS]
	str		r3, [sp, #sp_pout]	
	ldr		len, [sp, #arg_len]
	ldr		order, [sp, #arg_Order]
/*B_ALIGNMENT:*/	
	tst		r1, #0x3
	beq		3f/*S_ALIGNMENT*/
	ldrh	B_tmp, [r1], #2
	add		pB_tmp, sp, #sp_B
	sub		order, order, #1
0:
	subs	order, order, #1
	strh	B_tmp, [pB_tmp]
	ldrh	B_tmp, [r1], #2
	bgt		0b
	ldr		order, [sp, #arg_Order]
	strh	B_tmp, [pB_tmp]
	add		r1, sp, #sp_B						//R1 aligned pointer to B.
/*S_ALIGNMENT:*/
3:
	tst		r2, #0x3
	bne		4f/*MAIN_FORLOOP1*/
/*MAIN_FORLOOP0:	*/
0:
	mov		out32, #0
	ldr		order, [sp, #arg_Order]
/*MAIN_INNER_FORLOOP0:*/
	ldr		S_val, [r2], #2
	ldr		B_val, [r1], #4
	sub		order, order, #4
	mov		S_tmp2, S_val, lsr #16
	strh	S_val, [r2], #2
	SKP_SMLAD	out32, B_val, S_val, out32
	ldr		S_val, [r2]
1:
	ldr		B_val, [r1], #4
	add		S_tmp2, S_tmp2, S_val, lsl #16
	ldr		S_tmp1, [r2, #4]
	subs	order, order, #2
	str		S_tmp2, [r2], #4	
	SKP_SMLAD	out32, B_val, S_val, out32
	mov		S_tmp2, S_val, lsr #16
	mov		S_val, S_tmp1
	bgt		1b
	ldr		B_val, [r1]
	add		S_tmp2, S_tmp2, S_val, lsl #16
	SKP_SMLAD	out32, B_val, S_val, out32
	str		S_tmp2, [r2]
	
	ldrsh	in, [r0], #2
	ldr		r2, [sp, #sp_pS]
	ldr		r1, [sp, #sp_pB]
	mov		in_Q12, in, lsl #12
	qsub	out32, in_Q12, out32
	mov		out32, out32, asr #11
	strh	in, [r2]
	add		out32, out32, #1
#if EMBEDDED_ARM >=6
	ssat	out32, #16, out32, asr #1
#else	
	mov		out32, out32, asr #1
	cmp		out32, #0x8000
	mvnge	out32, #0x8000
	subge	out32, out32, #1
	cmn		out32, #0x8000
	movlt	out32, #0x8000
#endif
	subs	len, len, #1
	strh	out32, [r3], #2
	bgt		0b
	add		sp, sp, #48
	ldmia	sp!,  {r4-r10, fp, ip, pc}

/*MAIN_FORLOOP1:	//pS&3!=0*/
4:
0:
	mov		out32, #0
	ldr		order, [sp, #arg_Order]
/*MAIN_INNER_FORLOOP1:*/
	ldrh	S_val, [r2], #2
	ldr		S_tmp1, [r2]
	ldr		S_tmp2, [r2, #4]
	ldr		B_val, [r1], #4
	sub		order, order, #4
	add		S_val, S_val, S_tmp1, lsl #16
	mov		S_tmp1, S_tmp1, lsr #16
	str		S_val, [r2], #4
	SKP_SMLAD	out32, B_val, S_val, out32
1:
	ldr		B_val, [r1], #4
	add		S_val, S_tmp1, S_tmp2, lsl #16
	mov		S_tmp1, S_tmp2, lsr #16
	subs	order, order, #2
	ldrgt	S_tmp2, [r2, #4]
	str		S_val, [r2], #4	
	SKP_SMLAD	out32, B_val, S_val, out32
	bgt		1b
	ldrsh	S_tmp2, [r2]
	ldr		B_val, [r1]
	add		S_val, S_tmp1, S_tmp2, lsl #16
	SKP_SMLAD	out32, B_val, S_val, out32
	strh	S_val, [r2]
	
	ldrsh	in, [r0], #2
	ldr		r2, [sp, #sp_pS]
	ldr		r1, [sp, #sp_pB]
	mov		in_Q12, in, lsl #12
	qsub	out32, in_Q12, out32
	mov		out32, out32, asr #11
	strh	in, [r2]
	add		out32, out32, #1
#if EMBEDDED_ARM >=6
	ssat	out32, #16, out32, asr #1
#else	
	mov		out32, out32, asr #1
	cmp		out32, #0x8000
	mvnge	out32, #0x8000
	subge	out32, out32, #1
	cmn		out32, #0x8000
	movlt	out32, #0x8000
#endif
	subs	len, len, #1
	strh	out32, [r3], #2
	bgt		0b
	
	add		sp, sp, #48
	ldmia	sp!,  {r4-r10, fp, ip, pc}

#endif
#endif
