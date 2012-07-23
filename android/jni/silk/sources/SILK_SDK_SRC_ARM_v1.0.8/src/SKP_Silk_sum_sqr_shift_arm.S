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

x0		.req	r4
nrg_tmp		.req	r5
shift		.req	r6
nrg		.req	_r7
idx		.req	r8

.globl	SYM(SKP_Silk_sum_sqr_shift)
SYM(SKP_Silk_sum_sqr_shift):
	stmdb	sp!,  {r4-r8, fp, ip, lr}
	add		fp, sp, #28
  
  mov		idx, r3
  ands		nrg_tmp, r2, #2
  mov		nrg, #0
  ldrneh	x0, [r2], #2
  smulbbne	nrg, x0, x0
  subne		idx, idx, #1

  ldr		r4, [r2], #4
  mov		shift, #0
  sub		idx, idx, #1
0:
  subs		idx, idx, #2
  SKP_SMLAD	nrg, x0, x0, nrg
  ldrgt		x0, [r2], #4
  cmp		nrg, #0
  blt		1f
  cmp		idx, #0
  bgt 		0b
  beq		4f
  b		5f
1:
  mov		nrg, nrg, lsr #2
  mov		shift, #2
  cmp		idx, #0
  beq		4f
  blt		5f
3:
  subs		idx, idx, #2  
  SKP_SMUAD	nrg_tmp, x0, x0
  ldrgt		x0, [r2], #4
  adds		nrg, nrg, nrg_tmp, asr shift
  movlt		nrg, nrg, lsr #2
  addlt		shift, shift, #2
  cmp		idx, #0
  bgt		3b
  blt		5f
4:  
  ldrh		x0, [r2]
  smulbb	nrg_tmp, x0, x0
  add		nrg, nrg, nrg_tmp, asr shift
5:
  ands		nrg_tmp, nrg, #0xC0000000
  movne		nrg, nrg, lsr #2
  addne		shift, shift, #2
  str		shift, [r1]
  str		nrg, [r0]
  
	ldmia	sp!,  {r4-r8, fp, ip, pc}
#endif
#endif
