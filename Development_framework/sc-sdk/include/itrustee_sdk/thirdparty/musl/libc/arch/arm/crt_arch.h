#ifndef ARM_CRT_ARCH_H
#define ARM_CRT_ARCH_H

__asm__(
".text\n"
".global " START "\n"
".type " START ",%function\n"
START ": \n"
"	mov fp, #0\n"
"	mov lr, #0\n"
"	ldr r5, [r0, #8]\n"  /* paratbl  */
/* Little-endian */
"	str r1, [r5, #20]\n" /* PT_ZERO1 */
"	str r2, [r5, #24]\n" /* PT_ZERO2 */
"	ldr r1, 1f\n"
"	add r1, pc, r1\n"
"	mov r5, sp\n"
"2:	and r5, r5, #-16\n"
"	mov sp, r5\n"
"	bl " START "_c\n"
".weak _DYNAMIC\n"
".hidden _DYNAMIC\n"
".align 2\n"
"1:	.word _DYNAMIC-2b\n"
);

#endif