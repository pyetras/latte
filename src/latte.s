	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp1:
	.cfi_def_cfa_offset 16
	xorl	%edi, %edi
	callq	_printInt
	xorl	%eax, %eax
	popq	%rdx
	ret
	.cfi_endproc


.subsections_via_symbols
