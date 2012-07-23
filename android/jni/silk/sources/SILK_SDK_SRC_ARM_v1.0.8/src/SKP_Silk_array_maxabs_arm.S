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

#if EMBEDDED_ARM>=5

ptr_vec			.req	r0
val_vec			.req	r2
val_return		.req	r0
len				.req	r1
val1_max		.req	r3
val2_max		.req	r4
val_lvl			.req	r5
val_ind			.req	r6

const_int16_MAX	.req	r5

.globl	SYM(SKP_Silk_int16_array_maxabs)
SYM(SKP_Silk_int16_array_maxabs):
	stmdb	sp!,  {r4-r6, fp, ip, lr}
	add		fp, sp, #20

	cmp		r1, #6
	mvn		r5, #0						/*r5 = -1(max)*/
	blt		7f
	ands	r2, r0, #3
	bne		3f
	ands	r2, r1, #0x1
	bne		1f	
	
	ldr		val_vec, [ptr_vec], #4
	sub		len, len, #2
0:
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	ldr		val_vec, [ptr_vec], #4
	cmp		val1_max, r5
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #8					/*address*/
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #6						
	subs	len, len, #2
	bgt		0b
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #4					/*address*/
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #2						
	b		9f
/*Odd number of iterations*/	
1:
	ldr		val_vec, [ptr_vec], #4
	sub		len, len, #3
2:
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	ldr		val_vec, [ptr_vec], #4
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #8					/*address*/
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #6						
	subs	len, len, #2
	bgt		2b
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	ldrsh	val_vec, [ptr_vec], #2
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #6					/*address*/
	smulbb	val1_max, val_vec, val_vec
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #4
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #2					/*address*/
	b		9f						
/*Oddly aligned*/
3:
	ands	r2, r1, #0x1
	bne		5f
	
	ldrsh	val_vec, [ptr_vec], #2
	smulbb	val1_max, val_vec, val_vec
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #2
	ldr		val_vec, [ptr_vec], #4
	sub		len, len, #4
4:
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	ldr		val_vec, [ptr_vec], #4
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #8					/*address*/
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #6						
	subs	len, len, #2
	bgt		4b
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	ldrsh	val_vec, [ptr_vec], #2
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #6					/*address*/
	smulbb	val1_max, val_vec, val_vec
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #4
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #2					/*address*/
	b		9f
/*Odd number of iterations+Oddly aligned*/
5:	
	ldrsh	val_vec, [ptr_vec], #2
	smulbb	val1_max, val_vec, val_vec
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #2
	ldr		val_vec, [ptr_vec], #4
	sub		len, len, #3
6:
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	ldr		val_vec, [ptr_vec], #4
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #8					/*address*/
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #6						
	subs	len, len, #2
	bgt		6b
	smulbb	val1_max, val_vec, val_vec
	smultt	val2_max, val_vec, val_vec
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #4					/*address*/
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #2
	b		9f
/*Less than 6 iterations*/	
7:	
	ldrsh	val_vec, [ptr_vec], #2
	sub		len, len, #1	
8:
	smulbb	val1_max, val_vec, val_vec
	ldrsh	val_vec, [ptr_vec], #2
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #4
	subs	len, len, #1
	bgt		8b
	smulbb	val1_max, val_vec, val_vec
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #2
9:	
	ldrsh	val_return, [val_ind]
	mvn		const_int16_MAX, #0x80000000
	mov		const_int16_MAX, const_int16_MAX, asr #16
	cmp		val_return, #0
	rsblt	val_return, val_return, #0
	cmp		val_return, const_int16_MAX
	movge	val_return, const_int16_MAX
	
	ldmia	sp!,  {r4-r6, fp, ip, pc}

#elif EMBEDDED_ARM>=4	



ptr_vec			.req	r0
val1_vec		.req	r2
val2_vec		.req	r7
len				.req	r1
val1_max		.req	r3
val2_max		.req	r8
val_lvl			.req	r5
val_ind			.req	r6

val_return		.req	r0
const_int16_MAX	.req	r5

.globl	SYM(SKP_Silk_int16_array_maxabs)
SYM(SKP_Silk_int16_array_maxabs):
	stmdb	sp!,  {r4-r8, fp, ip, lr}
	add		fp, sp, #28
	
	mvn		val_lvl, #0						/*r5 = -1(max)*/

	ldrsh	val1_vec, [ptr_vec], #2
	ldrsh	val2_vec, [ptr_vec], #2
	sub		len, len, #2	
0:
	mul		val1_max, val1_vec, val1_vec
	mul		val2_max, val2_vec, val2_vec
	ldrsh	val1_vec, [ptr_vec], #2
	ldrsh	val2_vec, [ptr_vec], #2
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #8
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #6
	subs	len, len, #2
	bgt		0b
	mul		val1_max, val1_vec, val1_vec
	mul		val2_max, val2_vec, val2_vec
	cmp		val1_max, val_lvl
	movge	val_lvl, val1_max
	subge	val_ind, ptr_vec, #4
	cmn		len, #1						/*r1?=-1*/
	beq		1f
	cmp		val2_max, val_lvl
	movge	val_lvl, val2_max
	subge	val_ind, ptr_vec, #2
1:	
	ldrsh	val_return, [val_ind]
	mvn		const_int16_MAX, #0x80000000
	mov		const_int16_MAX, const_int16_MAX, asr #16
	cmp		val_return, #0
	rsblt	val_return, val_return, #0
	cmp		val_return, const_int16_MAX
	movge	val_return, const_int16_MAX
	
	ldmia	sp!,  {r4-r8, fp, ip, pc}
	
#endif
#endif
