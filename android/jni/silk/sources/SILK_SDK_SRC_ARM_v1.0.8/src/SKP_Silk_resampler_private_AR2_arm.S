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

len				.req	r4
S_0				.req	r5
S_1				.req	r6
in0				.req	_r7
out32				.req	r8
A_Q140				.req	sb
A_Q141				.req	sl

.set	sp_S, 0 
.globl	SYM(SKP_Silk_resampler_private_AR2)
SYM(SKP_Silk_resampler_private_AR2):
	stmdb	sp!,  {r4-r10, fp, ip, lr}
	add		fp, sp, #36
	sub		sp, sp, #4
.set	sp_len, 44 
	
	str	r0, [sp, #sp_S]
	ldr	S_0, [r0], #4
	ldr	S_1, [r0], #-4
	ldr	len, [sp, #sp_len]
	ldrsh	A_Q140, [r3], #2
	ldrsh	A_Q141, [r3]
	cmp	len, #0
	beq	1f

0:
	ldrsh	in0, [r2], #2
	add	out32, S_0, in0, lsl #8
	str	out32, [r1], #4
	subs	len, len, #1
	mov	out32, out32, lsl #2
	smlawb	S_0, out32, A_Q140, S_1
	smulwb	S_1, out32, A_Q141
	bgt	0b	
	
	str	S_0, [r0], #4
	str	S_1, [r0]
1:	
	add		sp, sp, #4
	ldmia	sp!,  {r4-r10, fp, ip, pc}
#endif
#endif
