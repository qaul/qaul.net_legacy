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
out				.req	r1
ind				.req	r2
coef0			.req	r3
coef1			.req	r4
coef2			.req	r5
#ifdef IPHONE
val0			.req	r6
val1			.req	r8
val2			.req	sb
val3			.req	_r7
#else
val0			.req	r6
val1			.req	_r7
val2			.req	r8
val3			.req	sb
#endif
tmpptr			.req	sl
tmp0			.req	ip
tmp1			.req	lr

.set	sp_buf, 0 
.set	sp_maxi, 4 
.globl	SYM(SKP_Silk_resampler_private_down_FIR_INTERPOL0)
SYM(SKP_Silk_resampler_private_down_FIR_INTERPOL0):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #8
.set	sp_inc, 48 
		cmp		r3, #0
		str		r1, [sp, #sp_buf]
		str		r3, [sp, #sp_maxi]
		ble		1f
		ldrh	r3, [r2], #2
		ldrh	r6, [r2], #2
		ldrh	r4, [r2], #2
		ldrh	_r7, [r2], #2
		ldrh	r5, [r2], #2
		ldrh	r8, [r2]
		mov		tmpptr, r1
		add		coef0, r3, r6, lsl #16
		add		coef1, r4, _r7, lsl #16
		add		coef2, r5, r8, lsl #16
		ldmia	tmpptr, {val0, val1}
		add		tmpptr, tmpptr, #40
		ldr		tmp0, [sp, #sp_inc]
		ldmia	tmpptr, {val2, val3}		
		mov		ind, #0
0:
		sub		tmpptr, tmpptr, #32
		add		ind, ind, tmp0	
		add		tmp0, val0, val3
		add		tmp1, val1, val2
		ldmia	tmpptr, {val0, val1}
		add		tmpptr, tmpptr, #24
		ldmia	tmpptr, {val2, val3}
		smulwb	out, tmp0, coef0
		smlawt	out, tmp1, coef0, out
		sub		tmpptr, tmpptr, #16
		add		tmp0, val0, val3
		add		tmp1, val1, val2
		ldmia	tmpptr, {val0, val1, val2, val3}
		smlawb	out, tmp0, coef1, out
		smlawt	out, tmp1, coef1, out
		ldr		tmpptr, [sp, #sp_buf]
		add		tmp0, val0, val3
		add		tmp1, val1, val2
		smlawb	out, tmp0, coef2, out
		smlawt	out, tmp1, coef2, out
		
		mov		tmp0, ind, asr #16
		mov		out, out, asr #5
		add		tmpptr, tmpptr, tmp0, lsl #2
		add		out, out, #1
		cmp		out, #0x10000
		mov		out, out, asr #1
		ldr		tmp1, [sp, #sp_maxi]
		movgt	out, #0x8000
		subge	out, out, #1
		cmn		out, #0x8000
		ldr		tmp0, [sp, #sp_inc]
		movlt	out, #0x8000
		cmp		ind, tmp1
		ldmltia	tmpptr, {val0, val1}
		strh	out, [r0], #2
		add		tmpptr, tmpptr, #40
		ldmltia	tmpptr, {val2, val3}
		blt		0b
1:		
	add		sp, sp, #8
	ldmia	sp!,  {r4-r10, fp, ip, pc}

#ifdef IPHONE
coefptr			.req	_r7
bufptr			.req	sl
#else
coefptr			.req	sb
bufptr			.req	sl
#endif

.set	sp_buf, 0 
.set	sp_coef, 4 
.set	sp_maxi, 8 
.globl	SYM(SKP_Silk_resampler_private_down_FIR_INTERPOL1)
SYM(SKP_Silk_resampler_private_down_FIR_INTERPOL1):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #12
.set	sp_inc, 52 
.set	sp_fir_fracs, 56 
		cmp		r3, #0
		str		r1, [sp, #sp_buf]
		str		r2, [sp, #sp_coef]
		str		r3, [sp, #sp_maxi]
		blt		1f
		mov		tmp1, #0xFF00
		mov		ind, #0
		add		tmp1, tmp1, #0xFF
		ldr		coefptr, [sp, #sp_coef]
		ldr		bufptr, [sp, #sp_buf]
0:		
		ldr		out, [sp, #sp_fir_fracs]
		mov		tmp0, ind, asr #16
		add		bufptr, bufptr, tmp0, lsl #2
		and 	tmp1, ind, tmp1
		smulwb	tmp1, tmp1, out
		ldmia	bufptr!, {val0, val1, val2}
		add		coefptr, coefptr, tmp1, lsl #3
		add		coefptr, coefptr, tmp1, lsl #2
		ldmia	coefptr, {coef0, coef1, coef2} 
		sub		tmp1, out, tmp1
		ldr		tmp0, [sp, #sp_inc]
		ldr		coefptr, [sp, #sp_coef]
		smulwb	out, val0, coef0
		smlawt	out, val1, coef0, out
		smlawb	out, val2, coef1, out
		ldmia	bufptr!, {val0, val1, val2}
		sub		tmp1, tmp1, #1
		add		ind, ind, tmp0
		add		coefptr, coefptr, tmp1, lsl #3
		add		coefptr, coefptr, tmp1, lsl #2
		smlawt	out, val0, coef1, out
		smlawb	out, val1, coef2, out
		smlawt	out, val2, coef2, out
		ldmia	coefptr, {coef0, coef1, coef2} 
		ldmia	bufptr!, {val0, val1, val2}
		ldr		tmp0, [sp, #sp_maxi]
		mov		tmp1, #0xFF00
		add		tmp1, tmp1, #0xFF
		smlawt	out, val0, coef2, out
		smlawb	out, val1, coef2, out
		smlawt	out, val2, coef1, out
		ldmia	bufptr!, {val0, val1, val2}
		ldr		coefptr, [sp, #sp_coef]
		ldr		bufptr, [sp, #sp_buf]
		smlawb	out, val0, coef1, out
		smlawt	out, val1, coef0, out
		smlawb	out, val2, coef0, out
		mov		out, out, asr #5
		add		out, out, #1
		cmp		out, #0x10000
		mov		out, out, asr #1
		movgt	out, #0x8000
		subge	out, out, #1
		cmn		out, #0x8000
		movlt	out, #0x8000
		cmp		ind, tmp0
		strh	out, [r0], #2
		blt		0b
1:		
	add		sp, sp, #12
	ldmia	sp!,  {r4-r10, fp, ip, pc}

#endif
#endif
