#ifndef AARCH64_CRT_ARCH_H
#define AARCH64_CRT_ARCH_H

__asm__(
".text\n"
".global " START "\n"
".type " START ",%function\n"
START ":\n"
"	mov x29, #0\n"
"	mov x30, #0\n"
"	ldr x18, [x0, #16]\n" /* paratbl  */
"	str x1, [x18, #40]\n" /* PT_ZERO1 */
"	str x2, [x18, #48]\n" /* PT_ZERO2 */
".weak _DYNAMIC\n"
".hidden _DYNAMIC\n"
"	adrp x1, _DYNAMIC\n"
"	add x1, x1, #:lo12:_DYNAMIC\n"
"	mov x18, sp\n"
"	and x18, x18, #-16\n"
"	mov sp, x18\n"
"	b " START "_c\n"
);

#endif