/*
 * Copyright 2020 Huawei Technologies Co., Ltd
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
    .text
    .file "llvm-link"
    .p2align 4        // -- Begin function sub_0_caller_wrapper
    .type sub_0_caller_wrapper,@function
sub_0_caller_wrapper:               // @sub_0_caller_wrapper
// %bb.0:
    and x8, x0, #0xffffffff
    str q0, [sp, #-16]!
    ldp x9, x10, [sp], #16
    add w11, w0, w1
    mov x0, x7
    stp x8, x11, [x6, #-16]
    stp x9, x10, [x6, #-32]
    ret
.Lfunc_end0:
    .size sub_0_caller_wrapper, .Lfunc_end0-sub_0_caller_wrapper
                                          // -- End function
    .globl caller                         // --Begin function caller
    .p2align 2
    .type caller,@function
caller:                                  //@caller
// %bb.0:
    stp x22, x21, [sp, #-32]!  // 16-byte Folded Spill
    stp x20, x19, [sp, #16]   // 16-byte Folded Spill
    mrs x8, TPIDR_EL0
    add x10, x8, :tprel_hi12:simulation_stack
    mov w9, #8388480
    add x10,x10, :tprel_lo12_nc:simulation_stack
    add x8, x8, :tprel_hi12:simulation_stack_pointer
    add x9, x10, x9
    adrp x10, __unnamed_1
    add x8, x8, :tprel_lo12_nc:simulation_stack_pointer
    add x10, x10, :lo12:__unnamed_1
    //APP
    stur x30, [sp, #-16]
    sub sp, sp, #16             // =16
    mov x21, x8
    mov x22, x9
    ldr x20, [x21]
    sub x20, x22, x20
    mov x6, sp
    str x6, [x20]
    mov sp, x20
    ldr x19, [x10]
    blr x19
    mov x20, sp
    sub x20, x22, x20
    str x20, [x21]
    ldr x19, [sp]
    mov sp, x19
    ldr x30, [sp]
    ldur    x0, [sp, #-8]
    ldur    x1, [sp, #-16]
    ldur    q0, [sp, #-32]
    add sp, sp, #16         //=16

    //NO_APP
    ldp x20, x19, [sp, #16]     // 16-byte Folded Reload
    ldp x22, x21, [sp], #32     // 16-byte Folded Reload
    ret
.Lfunc_end1:
    .size   caller, .Lfunc_end1-caller
                                    // -- End function
    .type   __unnamed_1,@object     //@0
    .section    .rodata, "a",@progbits
    .p2align    3
__unnamed_1:
    .xword sub_0_caller_wrapper
    .size   __unnamed_1, 8

    .type   simulation_stack,@object // @simulation_stack
    .section    .tbss.simulation_stack,"aGwT",@nobits,simulation_stack,comdat
    .globl simulation_stack
    .p2align    4
simulation_stack:
    .zero    8388608
    .size   simulation_stack, 8388608
    
    .type   simulation_stack_pointer,@object // @simulation_stack_pointer
    .section    .tbss.simulation_stack_pointer,"aGwT",@nobits,simulation_stack_pointer,comdat
    .globl simulation_stack_pointer
    .p2align    3
simulation_stack_pointer:
    .xword 0                        // 0x0
    .size simulation_stack_pointer, 8

    .ident  "clang version 10.0.1"
    .ident  "clang version 10.0.1"
    .section    ".note.GNU-stack","",@progbits
