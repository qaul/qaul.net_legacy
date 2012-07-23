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

len				.req	r0
S_0				.req	r3
S_1				.req	r4
S_2				.req	r5
S_3				.req	r6
in				.req	_r7
out1				.req	r8
coef01				.req	sb
coef23				.req	sl
coef45				.req	ip
coef6				.req	r8
coef_tmp			.req	_r7
out2				.req	lr

.set	sp_S, 0 
.set	sp_coef, 4 
.globl	SYM(SKP_Silk_resampler_private_ARMA4)
SYM(SKP_Silk_resampler_private_ARMA4):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #8
.set	sp_len, 48 
	
	ldr	lr, [sp, #sp_len]
	ldrh	coef01, [r3], #2
	ldrh	S_1, [r3], #2
	ldrh	coef23, [r3], #2
	ldrh	S_2, [r3], #2
	ldrh	coef45, [r3], #2
	ldrh	S_3, [r3], #2
	ldrh	coef6, [r3], #2
	cmp	lr, #0
	str	r0, [sp, #sp_S]
	beq	1f			// if len==0
	
	str	coef6, [sp, #sp_coef]
	ldrsh	in, [r2], #2
	add	coef01, coef01, S_1, lsl #16
	add	coef23, coef23, S_2, lsl #16
	add	coef45, coef45, S_3, lsl #16
	ldmia	r0, {S_0, S_1, S_2, S_3}
	mov	len, lr	
	
0:
	mov	in, in, lsl #8
	add	out1, in, S_0, lsl #2
	add	out2, out1, S_2, lsl #2
	
	smlawb	S_0, in, coef01, S_1
	smlawb	S_0, out1, coef23, S_0
	smlawt	S_2, out1, coef01, S_3
	smlawb	S_2, out2, coef45, S_2
	smulwt	S_1, out1, coef23
	smulwt	S_3, out2, coef45
	add	S_1, S_1, in, asr #2
	ldr	coef_tmp, [sp, #sp_coef]
	add	S_3, S_3, out1, asr #2
	
	mov	out1, #128
	smlawb	out1, out2, coef_tmp, out1
#if EMBEDDED_ARM<6	
	cmp	out1, #0x800000
	movge	out1, #0x800000
	subge	out1, out1, #1
	cmn	out1, #0x800000
	movlt	out1, #0x800000
	mov	out1, out1, asr #8
#else		
	ssat	out1, #16, out1, asr #8
#endif
	subs	len, len, #1
	strh	out1, [r1], #2
	ldrgtsh	in, [r2], #2
	bgt	0b
	
	ldr	r0, [sp, #sp_S]
	stmia	r0, {S_0, S_1, S_2, S_3}
1:	
	add		sp, sp, #8
	ldmia	sp!,  {r4-r10, fp, ip, pc}
#endif
#endif
