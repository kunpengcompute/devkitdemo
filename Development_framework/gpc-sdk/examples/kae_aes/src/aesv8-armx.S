#include "arm_arch.h"

#if __ARM_MAX_ARCH__>=7
.arch	armv8-a+crypto
.text
.align	5
.Lrcon:
.long	0x01,0x01,0x01,0x01
.long	0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d,0x0c0f0e0d	// rotate-n-splat
.long	0x1b,0x1b,0x1b,0x1b

.globl	aes_v8_set_encrypt_key
.type	aes_v8_set_encrypt_key,%function
.align	5
aes_v8_set_encrypt_key:
.Lenc_key:
	stp	x29,x30,[sp,#-16]!
	add	x29,sp,#0
	mov	x3,#-1
	cmp	x0,#0
	b.eq	.Lenc_key_abort
	cmp	x2,#0
	b.eq	.Lenc_key_abort
	mov	x3,#-2
	cmp	w1,#128
	b.lt	.Lenc_key_abort
	cmp	w1,#256
	b.gt	.Lenc_key_abort
	tst	w1,#0x3f
	b.ne	.Lenc_key_abort

	adr	x3,.Lrcon
	cmp	w1,#192

	eor	v0.16b,v0.16b,v0.16b
	ld1	{v3.16b},[x0],#16
	mov	w1,#8		// reuse w1
	ld1	{v1.4s,v2.4s},[x3],#32

	b.lt	.Loop128
	b.eq	.L192
	b	.L256

.align	4
.Loop128:
	tbl	v6.16b,{v3.16b},v2.16b
	ext	v5.16b,v0.16b,v3.16b,#12
	st1	{v3.4s},[x2],#16
	aese	v6.16b,v0.16b
	subs	w1,w1,#1

	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v6.16b,v6.16b,v1.16b
	eor	v3.16b,v3.16b,v5.16b
	shl	v1.16b,v1.16b,#1
	eor	v3.16b,v3.16b,v6.16b
	b.ne	.Loop128

	ld1	{v1.4s},[x3]

	tbl	v6.16b,{v3.16b},v2.16b
	ext	v5.16b,v0.16b,v3.16b,#12
	st1	{v3.4s},[x2],#16
	aese	v6.16b,v0.16b

	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v6.16b,v6.16b,v1.16b
	eor	v3.16b,v3.16b,v5.16b
	shl	v1.16b,v1.16b,#1
	eor	v3.16b,v3.16b,v6.16b

	tbl	v6.16b,{v3.16b},v2.16b
	ext	v5.16b,v0.16b,v3.16b,#12
	st1	{v3.4s},[x2],#16
	aese	v6.16b,v0.16b

	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v6.16b,v6.16b,v1.16b
	eor	v3.16b,v3.16b,v5.16b
	eor	v3.16b,v3.16b,v6.16b
	st1	{v3.4s},[x2]
	add	x2,x2,#0x50

	mov	w12,#10
	b	.Ldone

.align	4
.L192:
	ld1	{v4.8b},[x0],#8
	movi	v6.16b,#8			// borrow v6.16b
	st1	{v3.4s},[x2],#16
	sub	v2.16b,v2.16b,v6.16b	// adjust the mask

.Loop192:
	tbl	v6.16b,{v4.16b},v2.16b
	ext	v5.16b,v0.16b,v3.16b,#12
#ifdef __ARMEB__
	st1	{v4.4s},[x2],#16
	sub	x2,x2,#8
#else
	st1	{v4.8b},[x2],#8
#endif
	aese	v6.16b,v0.16b
	subs	w1,w1,#1

	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v3.16b,v3.16b,v5.16b

	dup	v5.4s,v3.s[3]
	eor	v5.16b,v5.16b,v4.16b
	eor	v6.16b,v6.16b,v1.16b
	ext	v4.16b,v0.16b,v4.16b,#12
	shl	v1.16b,v1.16b,#1
	eor	v4.16b,v4.16b,v5.16b
	eor	v3.16b,v3.16b,v6.16b
	eor	v4.16b,v4.16b,v6.16b
	st1	{v3.4s},[x2],#16
	b.ne	.Loop192

	mov	w12,#12
	add	x2,x2,#0x20
	b	.Ldone

.align	4
.L256:
	ld1	{v4.16b},[x0]
	mov	w1,#7
	mov	w12,#14
	st1	{v3.4s},[x2],#16

.Loop256:
	tbl	v6.16b,{v4.16b},v2.16b
	ext	v5.16b,v0.16b,v3.16b,#12
	st1	{v4.4s},[x2],#16
	aese	v6.16b,v0.16b
	subs	w1,w1,#1

	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v3.16b,v3.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v6.16b,v6.16b,v1.16b
	eor	v3.16b,v3.16b,v5.16b
	shl	v1.16b,v1.16b,#1
	eor	v3.16b,v3.16b,v6.16b
	st1	{v3.4s},[x2],#16
	b.eq	.Ldone

	dup	v6.4s,v3.s[3]		// just splat
	ext	v5.16b,v0.16b,v4.16b,#12
	aese	v6.16b,v0.16b

	eor	v4.16b,v4.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v4.16b,v4.16b,v5.16b
	ext	v5.16b,v0.16b,v5.16b,#12
	eor	v4.16b,v4.16b,v5.16b

	eor	v4.16b,v4.16b,v6.16b
	b	.Loop256

.Ldone:
	str	w12,[x2]
	mov	x3,#0

.Lenc_key_abort:
	mov	x0,x3			// return value
	ldr	x29,[sp],#16
	ret
.size	aes_v8_set_encrypt_key,.-aes_v8_set_encrypt_key

.globl	aes_v8_set_decrypt_key
.type	aes_v8_set_decrypt_key,%function
.align	5
aes_v8_set_decrypt_key:
.inst	0xd503233f		// paciasp
	stp	x29,x30,[sp,#-16]!
	add	x29,sp,#0
	bl	.Lenc_key

	cmp	x0,#0
	b.ne	.Ldec_key_abort

	sub	x2,x2,#240		// restore original x2
	mov	x4,#-16
	add	x0,x2,x12,lsl#4	// end of key schedule

	ld1	{v0.4s},[x2]
	ld1	{v1.4s},[x0]
	st1	{v0.4s},[x0],x4
	st1	{v1.4s},[x2],#16

.Loop_imc:
	ld1	{v0.4s},[x2]
	ld1	{v1.4s},[x0]
	aesimc	v0.16b,v0.16b
	aesimc	v1.16b,v1.16b
	st1	{v0.4s},[x0],x4
	st1	{v1.4s},[x2],#16
	cmp	x0,x2
	b.hi	.Loop_imc

	ld1	{v0.4s},[x2]
	aesimc	v0.16b,v0.16b
	st1	{v0.4s},[x0]

	eor	x0,x0,x0		// return value
.Ldec_key_abort:
	ldp	x29,x30,[sp],#16
.inst	0xd50323bf		// autiasp
	ret
.size	aes_v8_set_decrypt_key,.-aes_v8_set_decrypt_key
.globl	aes_v8_encrypt
.type	aes_v8_encrypt,%function
.align	5
aes_v8_encrypt:
	ldr	w3,[x2,#240]
	ld1	{v0.4s},[x2],#16
	ld1	{v2.16b},[x0]
	sub	w3,w3,#2
	ld1	{v1.4s},[x2],#16

.Loop_enc:
	aese	v2.16b,v0.16b
	aesmc	v2.16b,v2.16b
	ld1	{v0.4s},[x2],#16
	subs	w3,w3,#2
	aese	v2.16b,v1.16b
	aesmc	v2.16b,v2.16b
	ld1	{v1.4s},[x2],#16
	b.gt	.Loop_enc

	aese	v2.16b,v0.16b
	aesmc	v2.16b,v2.16b
	ld1	{v0.4s},[x2]
	aese	v2.16b,v1.16b
	eor	v2.16b,v2.16b,v0.16b

	st1	{v2.16b},[x1]
	ret
.size	aes_v8_encrypt,.-aes_v8_encrypt
.globl	aes_v8_ecb_encrypt
.type	aes_v8_ecb_encrypt,%function
.align	5
aes_v8_ecb_encrypt:
	subs	x2,x2,#16
	// Original input data size bigger than 16, jump to big size processing.
	b.ne	.Lecb_big_size
	ld1	{v0.16b},[x0]
	cmp	w4,#0					// en- or decrypting?
	ldr	w5,[x3,#240]
	ld1	{v5.4s,v6.4s},[x3],#32			// load key schedule...

	b.eq	.Lecb_small_dec
	aese	v0.16b,v5.16b
	aesmc	v0.16b,v0.16b
	ld1	{v16.4s,v17.4s},[x3],#32			// load key schedule...
	aese	v0.16b,v6.16b
	aesmc	v0.16b,v0.16b
	subs	w5,w5,#10			// if rounds==10, jump to aes-128-ecb processing
	b.eq	.Lecb_128_enc
.Lecb_round_loop:
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	ld1	{v16.4s},[x3],#16				// load key schedule...
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	ld1	{v17.4s},[x3],#16				// load key schedule...
	subs	w5,w5,#2			// bias
	b.gt	.Lecb_round_loop
.Lecb_128_enc:
	ld1	{v18.4s,v19.4s},[x3],#32		// load key schedule...
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	ld1	{v20.4s,v21.4s},[x3],#32		// load key schedule...
	aese	v0.16b,v18.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v19.16b
	aesmc	v0.16b,v0.16b
	ld1	{v22.4s,v23.4s},[x3],#32		// load key schedule...
	aese	v0.16b,v20.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v21.16b
	aesmc	v0.16b,v0.16b
	ld1	{v7.4s},[x3]
	aese	v0.16b,v22.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v23.16b
	eor	v0.16b,v0.16b,v7.16b
	st1	{v0.16b},[x1]
	b	.Lecb_Final_abort
.Lecb_small_dec:
	aesd	v0.16b,v5.16b
	aesimc	v0.16b,v0.16b
	ld1	{v16.4s,v17.4s},[x3],#32			// load key schedule...
	aesd	v0.16b,v6.16b
	aesimc	v0.16b,v0.16b
	subs	w5,w5,#10			// bias
	b.eq	.Lecb_128_dec
.Lecb_dec_round_loop:
	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	ld1	{v16.4s},[x3],#16				// load key schedule...
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	ld1	{v17.4s},[x3],#16				// load key schedule...
	subs	w5,w5,#2			// bias
	b.gt	.Lecb_dec_round_loop
.Lecb_128_dec:
	ld1	{v18.4s,v19.4s},[x3],#32		// load key schedule...
	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	ld1	{v20.4s,v21.4s},[x3],#32		// load key schedule...
	aesd	v0.16b,v18.16b
	aesimc	v0.16b,v0.16b
	aesd	v0.16b,v19.16b
	aesimc	v0.16b,v0.16b
	ld1	{v22.4s,v23.4s},[x3],#32		// load key schedule...
	aesd	v0.16b,v20.16b
	aesimc	v0.16b,v0.16b
	aesd	v0.16b,v21.16b
	aesimc	v0.16b,v0.16b
	ld1	{v7.4s},[x3]
	aesd	v0.16b,v22.16b
	aesimc	v0.16b,v0.16b
	aesd	v0.16b,v23.16b
	eor	v0.16b,v0.16b,v7.16b
	st1	{v0.16b},[x1]
	b	.Lecb_Final_abort
.Lecb_big_size:
	stp	x29,x30,[sp,#-16]!
	add	x29,sp,#0
	mov	x8,#16
	b.lo	.Lecb_done
	csel	x8,xzr,x8,eq

	cmp	w4,#0					// en- or decrypting?
	ldr	w5,[x3,#240]
	and	x2,x2,#-16
	ld1	{v0.16b},[x0],x8

	ld1	{v16.4s,v17.4s},[x3]				// load key schedule...
	sub	w5,w5,#6
	add	x7,x3,x5,lsl#4				// pointer to last 7 round keys
	sub	w5,w5,#2
	ld1	{v18.4s,v19.4s},[x7],#32
	ld1	{v20.4s,v21.4s},[x7],#32
	ld1	{v22.4s,v23.4s},[x7],#32
	ld1	{v7.4s},[x7]

	add	x7,x3,#32
	mov	w6,w5
	b.eq	.Lecb_dec

	ld1	{v1.16b},[x0],#16
	subs	x2,x2,#32				// bias
	add	w6,w5,#2
	orr	v3.16b,v1.16b,v1.16b
	orr	v24.16b,v1.16b,v1.16b
	orr	v1.16b,v0.16b,v0.16b
	b.lo	.Lecb_enc_tail

	orr	v1.16b,v3.16b,v3.16b
	ld1	{v24.16b},[x0],#16
	cmp	x2,#32
	b.lo	.Loop3x_ecb_enc

	ld1	{v25.16b},[x0],#16
	ld1	{v26.16b},[x0],#16
	sub	x2,x2,#32				// bias
	mov	w6,w5

.Loop5x_ecb_enc:
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v16.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v16.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v16.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v16.16b
	aesmc	v26.16b,v26.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v17.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v17.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v17.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v17.16b
	aesmc	v26.16b,v26.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Loop5x_ecb_enc

	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v16.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v16.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v16.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v16.16b
	aesmc	v26.16b,v26.16b
	cmp	x2,#0x40					// because .Lecb_enc_tail4x
	sub	x2,x2,#0x50

	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v17.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v17.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v17.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v17.16b
	aesmc	v26.16b,v26.16b
	csel	x6,xzr,x2,gt			// borrow x6, w6, "gt" is not typo
	mov	x7,x3

	aese	v0.16b,v18.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v18.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v18.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v18.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v18.16b
	aesmc	v26.16b,v26.16b
	add	x0,x0,x6				// x0 is adjusted in such way that
							// at exit from the loop v1.16b-v26.16b
							// are loaded with last "words"
	add	x6,x2,#0x60		    // because .Lecb_enc_tail4x

	aese	v0.16b,v19.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v19.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v19.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v19.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v19.16b
	aesmc	v26.16b,v26.16b

	aese	v0.16b,v20.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v20.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v20.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v20.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v20.16b
	aesmc	v26.16b,v26.16b

	aese	v0.16b,v21.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v21.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v21.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v21.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v21.16b
	aesmc	v26.16b,v26.16b

	aese	v0.16b,v22.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v22.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v22.16b
	aesmc	v24.16b,v24.16b
	aese	v25.16b,v22.16b
	aesmc	v25.16b,v25.16b
	aese	v26.16b,v22.16b
	aesmc	v26.16b,v26.16b

	aese	v0.16b,v23.16b
	ld1	{v2.16b},[x0],#16
	aese	v1.16b,v23.16b
	ld1	{v3.16b},[x0],#16
	aese	v24.16b,v23.16b
	ld1	{v27.16b},[x0],#16
	aese	v25.16b,v23.16b
	ld1	{v28.16b},[x0],#16
	aese	v26.16b,v23.16b
	ld1	{v29.16b},[x0],#16
	cbz	x6,.Lecb_enc_tail4x
	ld1	{v16.4s},[x7],#16			// re-pre-load rndkey[0]
	eor	v4.16b,v7.16b,v0.16b
	orr	v0.16b,v2.16b,v2.16b
	eor	v5.16b,v7.16b,v1.16b
	orr	v1.16b,v3.16b,v3.16b
	eor	v17.16b,v7.16b,v24.16b
	orr	v24.16b,v27.16b,v27.16b
	eor	v30.16b,v7.16b,v25.16b
	orr	v25.16b,v28.16b,v28.16b
	eor	v31.16b,v7.16b,v26.16b
	st1	{v4.16b},[x1],#16
	orr	v26.16b,v29.16b,v29.16b
	st1	{v5.16b},[x1],#16
	mov	w6,w5
	st1	{v17.16b},[x1],#16
	ld1	{v17.4s},[x7],#16			// re-pre-load rndkey[1]
	st1	{v30.16b},[x1],#16
	st1	{v31.16b},[x1],#16
	b.hs	.Loop5x_ecb_enc

	add	x2,x2,#0x50
	cbz	x2,.Lecb_done

	add	w6,w5,#2
	subs	x2,x2,#0x30
	orr	v0.16b,v27.16b,v27.16b
	orr	v1.16b,v28.16b,v28.16b
	orr	v24.16b,v29.16b,v29.16b
	b.lo	.Lecb_enc_tail

	b	.Loop3x_ecb_enc

.align	4
.Lecb_enc_tail4x:
	eor	v5.16b,v7.16b,v1.16b
	eor	v17.16b,v7.16b,v24.16b
	eor	v30.16b,v7.16b,v25.16b
	eor	v31.16b,v7.16b,v26.16b
	st1	{v5.16b},[x1],#16
	st1	{v17.16b},[x1],#16
	st1	{v30.16b},[x1],#16
	st1	{v31.16b},[x1],#16

	b	.Lecb_done
.align	4
.Loop3x_ecb_enc:
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v16.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v16.16b
	aesmc	v24.16b,v24.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v17.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v17.16b
	aesmc	v24.16b,v24.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Loop3x_ecb_enc

	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v16.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v16.16b
	aesmc	v24.16b,v24.16b
	subs	x2,x2,#0x30
	csel	x6,x2,x6,lo				// x6, w6, is zero at this point
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v17.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v17.16b
	aesmc	v24.16b,v24.16b
	add	x0,x0,x6			// x0 is adjusted in such way that
						// at exit from the loop v1.16b-v24.16b
						// are loaded with last "words"
	mov	x7,x3
	aese	v0.16b,v20.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v20.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v20.16b
	aesmc	v24.16b,v24.16b
	ld1	{v2.16b},[x0],#16
	aese	v0.16b,v21.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v21.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v21.16b
	aesmc	v24.16b,v24.16b
	ld1	{v3.16b},[x0],#16
	aese	v0.16b,v22.16b
	aesmc	v0.16b,v0.16b
	aese	v1.16b,v22.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v22.16b
	aesmc	v24.16b,v24.16b
	ld1	{v27.16b},[x0],#16
	aese	v0.16b,v23.16b
	aese	v1.16b,v23.16b
	aese	v24.16b,v23.16b
	ld1	{v16.4s},[x7],#16		// re-pre-load rndkey[0]
	add	w6,w5,#2
	eor	v4.16b,v7.16b,v0.16b
	eor	v5.16b,v7.16b,v1.16b
	eor	v24.16b,v24.16b,v7.16b
	ld1	{v17.4s},[x7],#16		// re-pre-load rndkey[1]
	st1	{v4.16b},[x1],#16
	orr	v0.16b,v2.16b,v2.16b
	st1	{v5.16b},[x1],#16
	orr	v1.16b,v3.16b,v3.16b
	st1	{v24.16b},[x1],#16
	orr	v24.16b,v27.16b,v27.16b
	b.hs	.Loop3x_ecb_enc

	cmn	x2,#0x30
	b.eq	.Lecb_done
	nop

.Lecb_enc_tail:
	aese	v1.16b,v16.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v16.16b
	aesmc	v24.16b,v24.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aese	v1.16b,v17.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v17.16b
	aesmc	v24.16b,v24.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Lecb_enc_tail

	aese	v1.16b,v16.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v16.16b
	aesmc	v24.16b,v24.16b
	aese	v1.16b,v17.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v17.16b
	aesmc	v24.16b,v24.16b
	aese	v1.16b,v20.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v20.16b
	aesmc	v24.16b,v24.16b
	cmn	x2,#0x20
	aese	v1.16b,v21.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v21.16b
	aesmc	v24.16b,v24.16b
	aese	v1.16b,v22.16b
	aesmc	v1.16b,v1.16b
	aese	v24.16b,v22.16b
	aesmc	v24.16b,v24.16b
	aese	v1.16b,v23.16b
	aese	v24.16b,v23.16b
	b.eq	.Lecb_enc_one
	eor	v5.16b,v7.16b,v1.16b
	eor	v17.16b,v7.16b,v24.16b
	st1	{v5.16b},[x1],#16
	st1	{v17.16b},[x1],#16
	b	.Lecb_done

.Lecb_enc_one:
	eor	v5.16b,v7.16b,v24.16b
	st1	{v5.16b},[x1],#16
	b	.Lecb_done
.align	5
.Lecb_dec:
	ld1	{v1.16b},[x0],#16
	subs	x2,x2,#32			// bias
	add	w6,w5,#2
	orr	v3.16b,v1.16b,v1.16b
	orr	v24.16b,v1.16b,v1.16b
	orr	v1.16b,v0.16b,v0.16b
	b.lo	.Lecb_dec_tail

	orr	v1.16b,v3.16b,v3.16b
	ld1	{v24.16b},[x0],#16
	cmp	x2,#32
	b.lo	.Loop3x_ecb_dec

	ld1	{v25.16b},[x0],#16
	ld1	{v26.16b},[x0],#16
	sub	x2,x2,#32				// bias
	mov	w6,w5

.Loop5x_ecb_dec:
	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v16.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v16.16b
	aesimc	v26.16b,v26.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v17.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v17.16b
	aesimc	v26.16b,v26.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Loop5x_ecb_dec

	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v16.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v16.16b
	aesimc	v26.16b,v26.16b
	cmp	x2,#0x40				// because .Lecb_tail4x
	sub	x2,x2,#0x50

	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v17.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v17.16b
	aesimc	v26.16b,v26.16b
	csel	x6,xzr,x2,gt		// borrow x6, w6, "gt" is not typo
	mov	x7,x3

	aesd	v0.16b,v18.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v18.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v18.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v18.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v18.16b
	aesimc	v26.16b,v26.16b
	add	x0,x0,x6				// x0 is adjusted in such way that
							// at exit from the loop v1.16b-v26.16b
							// are loaded with last "words"
	add	x6,x2,#0x60			// because .Lecb_tail4x

	aesd	v0.16b,v19.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v19.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v19.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v19.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v19.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v20.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v20.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v20.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v20.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v20.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v21.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v21.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v21.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v21.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v21.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v22.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v22.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v22.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v22.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v22.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v23.16b
	ld1	{v2.16b},[x0],#16
	aesd	v1.16b,v23.16b
	ld1	{v3.16b},[x0],#16
	aesd	v24.16b,v23.16b
	ld1	{v27.16b},[x0],#16
	aesd	v25.16b,v23.16b
	ld1	{v28.16b},[x0],#16
	aesd	v26.16b,v23.16b
	ld1	{v29.16b},[x0],#16
	cbz	x6,.Lecb_tail4x
	ld1	{v16.4s},[x7],#16			// re-pre-load rndkey[0]
	eor	v4.16b,v7.16b,v0.16b
	orr	v0.16b,v2.16b,v2.16b
	eor	v5.16b,v7.16b,v1.16b
	orr	v1.16b,v3.16b,v3.16b
	eor	v17.16b,v7.16b,v24.16b
	orr	v24.16b,v27.16b,v27.16b
	eor	v30.16b,v7.16b,v25.16b
	orr	v25.16b,v28.16b,v28.16b
	eor	v31.16b,v7.16b,v26.16b
	st1	{v4.16b},[x1],#16
	orr	v26.16b,v29.16b,v29.16b
	st1	{v5.16b},[x1],#16
	mov	w6,w5
	st1	{v17.16b},[x1],#16
	ld1	{v17.4s},[x7],#16			// re-pre-load rndkey[1]
	st1	{v30.16b},[x1],#16
	st1	{v31.16b},[x1],#16
	b.hs	.Loop5x_ecb_dec

	add	x2,x2,#0x50
	cbz	x2,.Lecb_done

	add	w6,w5,#2
	subs	x2,x2,#0x30
	orr	v0.16b,v27.16b,v27.16b
	orr	v1.16b,v28.16b,v28.16b
	orr	v24.16b,v29.16b,v29.16b
	b.lo	.Lecb_dec_tail

	b	.Loop3x_ecb_dec

.align	4
.Lecb_tail4x:
	eor	v5.16b,v7.16b,v1.16b
	eor	v17.16b,v7.16b,v24.16b
	eor	v30.16b,v7.16b,v25.16b
	eor	v31.16b,v7.16b,v26.16b
	st1	{v5.16b},[x1],#16
	st1	{v17.16b},[x1],#16
	st1	{v30.16b},[x1],#16
	st1	{v31.16b},[x1],#16

	b	.Lecb_done
.align	4
.Loop3x_ecb_dec:
	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Loop3x_ecb_dec

	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	subs	x2,x2,#0x30
	csel	x6,x2,x6,lo				// x6, w6, is zero at this point
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	add	x0,x0,x6 			// x0 is adjusted in such way that
						// at exit from the loop v1.16b-v24.16b
						// are loaded with last "words"
	mov	x7,x3
	aesd	v0.16b,v20.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v20.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v20.16b
	aesimc	v24.16b,v24.16b
	ld1	{v2.16b},[x0],#16
	aesd	v0.16b,v21.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v21.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v21.16b
	aesimc	v24.16b,v24.16b
	ld1	{v3.16b},[x0],#16
	aesd	v0.16b,v22.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v22.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v22.16b
	aesimc	v24.16b,v24.16b
	ld1	{v27.16b},[x0],#16
	aesd	v0.16b,v23.16b
	aesd	v1.16b,v23.16b
	aesd	v24.16b,v23.16b
	ld1	{v16.4s},[x7],#16			// re-pre-load rndkey[0]
	add	w6,w5,#2
	eor	v4.16b,v7.16b,v0.16b
	eor	v5.16b,v7.16b,v1.16b
	eor	v24.16b,v24.16b,v7.16b
	ld1	{v17.4s},[x7],#16			// re-pre-load rndkey[1]
	st1	{v4.16b},[x1],#16
	orr	v0.16b,v2.16b,v2.16b
	st1	{v5.16b},[x1],#16
	orr	v1.16b,v3.16b,v3.16b
	st1	{v24.16b},[x1],#16
	orr	v24.16b,v27.16b,v27.16b
	b.hs	.Loop3x_ecb_dec

	cmn	x2,#0x30
	b.eq	.Lecb_done
	nop

.Lecb_dec_tail:
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Lecb_dec_tail

	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	aesd	v1.16b,v20.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v20.16b
	aesimc	v24.16b,v24.16b
	cmn	x2,#0x20
	aesd	v1.16b,v21.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v21.16b
	aesimc	v24.16b,v24.16b
	aesd	v1.16b,v22.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v22.16b
	aesimc	v24.16b,v24.16b
	aesd	v1.16b,v23.16b
	aesd	v24.16b,v23.16b
	b.eq	.Lecb_dec_one
	eor	v5.16b,v7.16b,v1.16b
	eor	v17.16b,v7.16b,v24.16b
	st1	{v5.16b},[x1],#16
	st1	{v17.16b},[x1],#16
	b	.Lecb_done

.Lecb_dec_one:
	eor	v5.16b,v7.16b,v24.16b
	st1	{v5.16b},[x1],#16

.Lecb_done:
	ldr	x29,[sp],#16
.Lecb_Final_abort:
	ret
.size	aes_v8_ecb_encrypt,.-aes_v8_ecb_encrypt
.globl	aes_v8_cbc_encrypt
.type	aes_v8_cbc_encrypt,%function
.align	5
aes_v8_cbc_encrypt:
	stp	x29,x30,[sp,#-16]!
	add	x29,sp,#0
	subs	x2,x2,#16
	mov	x8,#16
	b.lo	.Lcbc_abort
	csel	x8,xzr,x8,eq

	cmp	w5,#0			// en- or decrypting?
	ldr	w5,[x3,#240]
	and	x2,x2,#-16
	ld1	{v6.16b},[x4]
	ld1	{v0.16b},[x0],x8

	ld1	{v16.4s,v17.4s},[x3]		// load key schedule...
	sub	w5,w5,#6
	add	x7,x3,x5,lsl#4	// pointer to last 7 round keys
	sub	w5,w5,#2
	ld1	{v18.4s,v19.4s},[x7],#32
	ld1	{v20.4s,v21.4s},[x7],#32
	ld1	{v22.4s,v23.4s},[x7],#32
	ld1	{v7.4s},[x7]

	add	x7,x3,#32
	mov	w6,w5
	b.eq	.Lcbc_dec

	cmp	w5,#2
	eor	v0.16b,v0.16b,v6.16b
	eor	v5.16b,v16.16b,v7.16b
	b.eq	.Lcbc_enc128

	ld1	{v2.4s,v3.4s},[x7]
	add	x7,x3,#16
	add	x6,x3,#16*4
	add	x12,x3,#16*5
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	add	x14,x3,#16*6
	add	x3,x3,#16*7
	b	.Lenter_cbc_enc

.align	4
.Loop_cbc_enc:
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	st1	{v6.16b},[x1],#16
.Lenter_cbc_enc:
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v2.16b
	aesmc	v0.16b,v0.16b
	ld1	{v16.4s},[x6]
	cmp	w5,#4
	aese	v0.16b,v3.16b
	aesmc	v0.16b,v0.16b
	ld1	{v17.4s},[x12]
	b.eq	.Lcbc_enc192

	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	ld1	{v16.4s},[x14]
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	ld1	{v17.4s},[x3]
	nop

.Lcbc_enc192:
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	subs	x2,x2,#16
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	csel	x8,xzr,x8,eq
	aese	v0.16b,v18.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v19.16b
	aesmc	v0.16b,v0.16b
	ld1	{v16.16b},[x0],x8
	aese	v0.16b,v20.16b
	aesmc	v0.16b,v0.16b
	eor	v16.16b,v16.16b,v5.16b
	aese	v0.16b,v21.16b
	aesmc	v0.16b,v0.16b
	ld1	{v17.4s},[x7]		// re-pre-load rndkey[1]
	aese	v0.16b,v22.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v23.16b
	eor	v6.16b,v0.16b,v7.16b
	b.hs	.Loop_cbc_enc

	st1	{v6.16b},[x1],#16
	b	.Lcbc_done

.align	5
.Lcbc_enc128:
	ld1	{v2.4s,v3.4s},[x7]
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	b	.Lenter_cbc_enc128
.Loop_cbc_enc128:
	aese	v0.16b,v16.16b
	aesmc	v0.16b,v0.16b
	st1	{v6.16b},[x1],#16
.Lenter_cbc_enc128:
	aese	v0.16b,v17.16b
	aesmc	v0.16b,v0.16b
	subs	x2,x2,#16
	aese	v0.16b,v2.16b
	aesmc	v0.16b,v0.16b
	csel	x8,xzr,x8,eq
	aese	v0.16b,v3.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v18.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v19.16b
	aesmc	v0.16b,v0.16b
	ld1	{v16.16b},[x0],x8
	aese	v0.16b,v20.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v21.16b
	aesmc	v0.16b,v0.16b
	aese	v0.16b,v22.16b
	aesmc	v0.16b,v0.16b
	eor	v16.16b,v16.16b,v5.16b
	aese	v0.16b,v23.16b
	eor	v6.16b,v0.16b,v7.16b
	b.hs	.Loop_cbc_enc128

	st1	{v6.16b},[x1],#16
	b	.Lcbc_done
.align	5
.Lcbc_dec:
	ld1	{v24.16b},[x0],#16
	subs	x2,x2,#32		// bias
	add	w6,w5,#2
	orr	v3.16b,v0.16b,v0.16b
	orr	v1.16b,v0.16b,v0.16b
	orr	v27.16b,v24.16b,v24.16b
	b.lo	.Lcbc_dec_tail

	orr	v1.16b,v24.16b,v24.16b
	ld1	{v24.16b},[x0],#16
	orr	v2.16b,v0.16b,v0.16b
	orr	v3.16b,v1.16b,v1.16b
	orr	v27.16b,v24.16b,v24.16b
	cmp	x2,#32
	b.lo	.Loop3x_cbc_dec

	ld1	{v25.16b},[x0],#16
	ld1	{v26.16b},[x0],#16
	sub	x2,x2,#32		// bias
	mov	w6,w5
	orr	v28.16b,v25.16b,v25.16b
	orr	v29.16b,v26.16b,v26.16b

.Loop5x_cbc_dec:
	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v16.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v16.16b
	aesimc	v26.16b,v26.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v17.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v17.16b
	aesimc	v26.16b,v26.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Loop5x_cbc_dec

	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v16.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v16.16b
	aesimc	v26.16b,v26.16b
	cmp	x2,#0x40		// because .Lcbc_tail4x
	sub	x2,x2,#0x50

	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v17.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v17.16b
	aesimc	v26.16b,v26.16b
	csel	x6,xzr,x2,gt		// borrow x6, w6, "gt" is not typo
	mov	x7,x3

	aesd	v0.16b,v18.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v18.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v18.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v18.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v18.16b
	aesimc	v26.16b,v26.16b
	add	x0,x0,x6		// x0 is adjusted in such way that
					// at exit from the loop v1.16b-v26.16b
					// are loaded with last "words"
	add	x6,x2,#0x60		// because .Lcbc_tail4x

	aesd	v0.16b,v19.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v19.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v19.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v19.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v19.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v20.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v20.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v20.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v20.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v20.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v21.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v21.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v21.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v21.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v21.16b
	aesimc	v26.16b,v26.16b

	aesd	v0.16b,v22.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v22.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v22.16b
	aesimc	v24.16b,v24.16b
	aesd	v25.16b,v22.16b
	aesimc	v25.16b,v25.16b
	aesd	v26.16b,v22.16b
	aesimc	v26.16b,v26.16b

	eor	v4.16b,v6.16b,v7.16b
	aesd	v0.16b,v23.16b
	eor	v5.16b,v2.16b,v7.16b
	ld1	{v2.16b},[x0],#16
	aesd	v1.16b,v23.16b
	eor	v17.16b,v3.16b,v7.16b
	ld1	{v3.16b},[x0],#16
	aesd	v24.16b,v23.16b
	eor	v30.16b,v27.16b,v7.16b
	ld1	{v27.16b},[x0],#16
	aesd	v25.16b,v23.16b
	eor	v31.16b,v28.16b,v7.16b
	ld1	{v28.16b},[x0],#16
	aesd	v26.16b,v23.16b
	orr	v6.16b,v29.16b,v29.16b
	ld1	{v29.16b},[x0],#16
	cbz	x6,.Lcbc_tail4x
	ld1	{v16.4s},[x7],#16	// re-pre-load rndkey[0]
	eor	v4.16b,v4.16b,v0.16b
	orr	v0.16b,v2.16b,v2.16b
	eor	v5.16b,v5.16b,v1.16b
	orr	v1.16b,v3.16b,v3.16b
	eor	v17.16b,v17.16b,v24.16b
	orr	v24.16b,v27.16b,v27.16b
	eor	v30.16b,v30.16b,v25.16b
	orr	v25.16b,v28.16b,v28.16b
	eor	v31.16b,v31.16b,v26.16b
	st1	{v4.16b},[x1],#16
	orr	v26.16b,v29.16b,v29.16b
	st1	{v5.16b},[x1],#16
	mov	w6,w5
	st1	{v17.16b},[x1],#16
	ld1	{v17.4s},[x7],#16	// re-pre-load rndkey[1]
	st1	{v30.16b},[x1],#16
	st1	{v31.16b},[x1],#16
	b.hs	.Loop5x_cbc_dec

	add	x2,x2,#0x50
	cbz	x2,.Lcbc_done

	add	w6,w5,#2
	subs	x2,x2,#0x30
	orr	v0.16b,v27.16b,v27.16b
	orr	v2.16b,v27.16b,v27.16b
	orr	v1.16b,v28.16b,v28.16b
	orr	v3.16b,v28.16b,v28.16b
	orr	v24.16b,v29.16b,v29.16b
	orr	v27.16b,v29.16b,v29.16b
	b.lo	.Lcbc_dec_tail

	b	.Loop3x_cbc_dec

.align	4
.Lcbc_tail4x:
	eor	v5.16b,v4.16b,v1.16b
	eor	v17.16b,v17.16b,v24.16b
	eor	v30.16b,v30.16b,v25.16b
	eor	v31.16b,v31.16b,v26.16b
	st1	{v5.16b},[x1],#16
	st1	{v17.16b},[x1],#16
	st1	{v30.16b},[x1],#16
	st1	{v31.16b},[x1],#16

	b	.Lcbc_done
.align	4
.Loop3x_cbc_dec:
	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Loop3x_cbc_dec

	aesd	v0.16b,v16.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	eor	v4.16b,v6.16b,v7.16b
	subs	x2,x2,#0x30
	eor	v5.16b,v2.16b,v7.16b
	csel	x6,x2,x6,lo			// x6, w6, is zero at this point
	aesd	v0.16b,v17.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	eor	v17.16b,v3.16b,v7.16b
	add	x0,x0,x6		// x0 is adjusted in such way that
					// at exit from the loop v1.16b-v24.16b
					// are loaded with last "words"
	orr	v6.16b,v27.16b,v27.16b
	mov	x7,x3
	aesd	v0.16b,v20.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v20.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v20.16b
	aesimc	v24.16b,v24.16b
	ld1	{v2.16b},[x0],#16
	aesd	v0.16b,v21.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v21.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v21.16b
	aesimc	v24.16b,v24.16b
	ld1	{v3.16b},[x0],#16
	aesd	v0.16b,v22.16b
	aesimc	v0.16b,v0.16b
	aesd	v1.16b,v22.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v22.16b
	aesimc	v24.16b,v24.16b
	ld1	{v27.16b},[x0],#16
	aesd	v0.16b,v23.16b
	aesd	v1.16b,v23.16b
	aesd	v24.16b,v23.16b
	ld1	{v16.4s},[x7],#16	// re-pre-load rndkey[0]
	add	w6,w5,#2
	eor	v4.16b,v4.16b,v0.16b
	eor	v5.16b,v5.16b,v1.16b
	eor	v24.16b,v24.16b,v17.16b
	ld1	{v17.4s},[x7],#16	// re-pre-load rndkey[1]
	st1	{v4.16b},[x1],#16
	orr	v0.16b,v2.16b,v2.16b
	st1	{v5.16b},[x1],#16
	orr	v1.16b,v3.16b,v3.16b
	st1	{v24.16b},[x1],#16
	orr	v24.16b,v27.16b,v27.16b
	b.hs	.Loop3x_cbc_dec

	cmn	x2,#0x30
	b.eq	.Lcbc_done
	nop

.Lcbc_dec_tail:
	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	ld1	{v16.4s},[x7],#16
	subs	w6,w6,#2
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	ld1	{v17.4s},[x7],#16
	b.gt	.Lcbc_dec_tail

	aesd	v1.16b,v16.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v16.16b
	aesimc	v24.16b,v24.16b
	aesd	v1.16b,v17.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v17.16b
	aesimc	v24.16b,v24.16b
	aesd	v1.16b,v20.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v20.16b
	aesimc	v24.16b,v24.16b
	cmn	x2,#0x20
	aesd	v1.16b,v21.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v21.16b
	aesimc	v24.16b,v24.16b
	eor	v5.16b,v6.16b,v7.16b
	aesd	v1.16b,v22.16b
	aesimc	v1.16b,v1.16b
	aesd	v24.16b,v22.16b
	aesimc	v24.16b,v24.16b
	eor	v17.16b,v3.16b,v7.16b
	aesd	v1.16b,v23.16b
	aesd	v24.16b,v23.16b
	b.eq	.Lcbc_dec_one
	eor	v5.16b,v5.16b,v1.16b
	eor	v17.16b,v17.16b,v24.16b
	orr	v6.16b,v27.16b,v27.16b
	st1	{v5.16b},[x1],#16
	st1	{v17.16b},[x1],#16
	b	.Lcbc_done

.Lcbc_dec_one:
	eor	v5.16b,v5.16b,v24.16b
	orr	v6.16b,v27.16b,v27.16b
	st1	{v5.16b},[x1],#16

.Lcbc_done:
	st1	{v6.16b},[x4]
.Lcbc_abort:
	ldr	x29,[sp],#16
	ret
.size	aes_v8_cbc_encrypt,.-aes_v8_cbc_encrypt
#endif
