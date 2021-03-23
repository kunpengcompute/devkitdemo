    .file "test.s"
    .text
    .global caller
    .type caller, @funciton
caller:
.LFB0:
    .cfi_startproc
    pushq %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movq %rsp, %rbp
    .cfi_def_cfa_register 6
    movl %edi, -4(%rbp)
    movl %esi, -8(%rbp)
    movl -8(%rbp), %eax
    movl -4(%rbp), %edx
    addl %edx, %eax
    popq %rbp
    .cfi_def_cfa 7, 8
    ret
    .cfi_endproc
.LFE0:
    .size caller, .caller
    .ident "GCC: (GNU) 4.8.5 20150623 (NeoKylin 4.8.5-36)"
    .section .note.GNU-stack,"",@progbits
    
