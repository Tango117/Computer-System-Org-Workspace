	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15, 4
	.globl	_yes_response_c         ## -- Begin function yes_response_c
	.p2align	4, 0x90
_yes_response_c:                        ## @yes_response_c
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$224, %rsp
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	leaq	-208(%rbp), %rdi
	movl	$200, %esi
	callq	_read_line
	leaq	-208(%rbp), %rdi
	leaq	L_.str(%rip), %rsi
	movl	%eax, -216(%rbp)        ## 4-byte Spill
	callq	_strcasecmp
	cmpl	$0, %eax
	je	LBB0_3
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	leaq	-208(%rbp), %rdi
	leaq	L_.str.1(%rip), %rsi
	callq	_strcasecmp
	cmpl	$0, %eax
	jne	LBB0_4
LBB0_3:
	movl	$1, -212(%rbp)
	jmp	LBB0_9
LBB0_4:                                 ##   in Loop: Header=BB0_1 Depth=1
	leaq	-208(%rbp), %rdi
	leaq	L_.str.2(%rip), %rsi
	callq	_strcasecmp
	cmpl	$0, %eax
	je	LBB0_6
## %bb.5:                               ##   in Loop: Header=BB0_1 Depth=1
	leaq	-208(%rbp), %rdi
	leaq	L_.str.3(%rip), %rsi
	callq	_strcasecmp
	cmpl	$0, %eax
	jne	LBB0_7
LBB0_6:
	movl	$0, -212(%rbp)
	jmp	LBB0_9
LBB0_7:                                 ##   in Loop: Header=BB0_1 Depth=1
	jmp	LBB0_8
LBB0_8:                                 ##   in Loop: Header=BB0_1 Depth=1
	leaq	L_.str.4(%rip), %rdi
	movb	$0, %al
	callq	_printf
	jmp	LBB0_1
LBB0_9:
	movl	-212(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	-8(%rbp), %rdx
	cmpq	%rdx, %rcx
	movl	%eax, -220(%rbp)        ## 4-byte Spill
	jne	LBB0_11
## %bb.10:
	movl	-220(%rbp), %eax        ## 4-byte Reload
	addq	$224, %rsp
	popq	%rbp
	retq
LBB0_11:
	callq	___stack_chk_fail
	ud2
	.cfi_endproc
                                        ## -- End function
	.globl	_new_node_c             ## -- Begin function new_node_c
	.p2align	4, 0x90
_new_node_c:                            ## @new_node_c
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp) 				##
	movl	$216, %edi
	callq	_malloc
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	$0, 208(%rax)
	movq	-16(%rbp), %rax
	movq	$0, 200(%rax)
	movq	-16(%rbp), %rdi
	movq	-8(%rbp), %rsi
	movl	$200, %edx
	callq	___strcpy_chk
	movq	-16(%rbp), %rcx
	movq	%rax, -24(%rbp)         ## 8-byte Spill
	movq	%rcx, %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_guess_animal_c         ## -- Begin function guess_animal_c
	.p2align	4, 0x90
_guess_animal_c:                        ## @guess_animal_c
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$272, %rsp              ## imm = 0x110
	movq	_root@GOTPCREL(%rip), %rax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rcx
	movq	%rcx, -8(%rbp)
	cmpq	$0, (%rax)
	jne	LBB2_2
## %bb.1:
	movl	$216, %edi
	callq	_malloc
	movq	%rax, -216(%rbp)
	movq	-216(%rbp), %rax
	movq	$0, 208(%rax)
	movq	-216(%rbp), %rax
	movq	$0, 200(%rax)
	leaq	L_.str.5(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movq	-216(%rbp), %rdi
	movl	$200, %esi
	movl	%eax, -228(%rbp)        ## 4-byte Spill
	callq	_read_line
	movq	_root@GOTPCREL(%rip), %rcx
	movq	-216(%rbp), %rdx
	movq	%rdx, (%rcx)
	jmp	LBB2_17
LBB2_2:
	movq	_root@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -216(%rbp)
LBB2_3:                                 ## =>This Inner Loop Header: Depth=1
	movq	-216(%rbp), %rax
	cmpq	$0, 200(%rax)
	jne	LBB2_11
## %bb.4:                               ##   in Loop: Header=BB2_3 Depth=1
	movq	-216(%rbp), %rax
	cmpq	$0, 208(%rax)
	jne	LBB2_11
## %bb.5:
	movq	-216(%rbp), %rsi
	leaq	L_.str.6(%rip), %rdi
	movb	$0, %al
	callq	_printf
	leaq	L_.str.7(%rip), %rdi
	movl	%eax, -232(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	movl	%eax, -236(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_yes_response
	cmpl	$0, %eax
	je	LBB2_7
## %bb.6:
	leaq	L_.str.8(%rip), %rdi
	movb	$0, %al
	callq	_printf
	jmp	LBB2_16
LBB2_7:
	leaq	L_.str.9(%rip), %rdi
	movb	$0, %al
	callq	_printf
	leaq	-208(%rbp), %rdi
	movl	$200, %esi
	movl	%eax, -240(%rbp)        ## 4-byte Spill
	callq	_read_line
	leaq	-208(%rbp), %rdi
	movl	%eax, -244(%rbp)        ## 4-byte Spill
	callq	_new_node_c
	leaq	-208(%rbp), %rsi
	movq	%rax, -224(%rbp)
	movq	-216(%rbp), %rdx
	leaq	L_.str.10(%rip), %rdi
	movb	$0, %al
	callq	_printf
	leaq	-208(%rbp), %rdi
	movl	$200, %esi
	movl	%eax, -248(%rbp)        ## 4-byte Spill
	callq	_read_line
	movq	-224(%rbp), %rsi
	leaq	L_.str.11(%rip), %rdi
	movl	%eax, -252(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	movl	%eax, -256(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_yes_response
	cmpl	$0, %eax
	je	LBB2_9
## %bb.8:
	movq	-224(%rbp), %rax
	movq	-216(%rbp), %rcx
	movq	%rax, 200(%rcx)
	movq	-216(%rbp), %rdi
	callq	_new_node_c
	movq	-216(%rbp), %rcx
	movq	%rax, 208(%rcx)
	jmp	LBB2_10
LBB2_9:
	movq	-224(%rbp), %rax
	movq	-216(%rbp), %rcx
	movq	%rax, 208(%rcx)
	movq	-216(%rbp), %rdi
	callq	_new_node_c
	movq	-216(%rbp), %rcx
	movq	%rax, 200(%rcx)
LBB2_10:
	leaq	-208(%rbp), %rsi
	movq	-216(%rbp), %rdi
	movl	$200, %edx
	callq	___strcpy_chk
	jmp	LBB2_16
LBB2_11:                                ##   in Loop: Header=BB2_3 Depth=1
	movq	-216(%rbp), %rsi
	leaq	L_.str.12(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	%eax, -260(%rbp)        ## 4-byte Spill
	movb	$0, %al
	callq	_yes_response
	cmpl	$0, %eax
	je	LBB2_13
## %bb.12:                              ##   in Loop: Header=BB2_3 Depth=1
	movq	-216(%rbp), %rax
	movq	200(%rax), %rax
	movq	%rax, -216(%rbp)
	jmp	LBB2_14
LBB2_13:                                ##   in Loop: Header=BB2_3 Depth=1
	movq	-216(%rbp), %rax
	movq	208(%rax), %rax
	movq	%rax, -216(%rbp)
LBB2_14:                                ##   in Loop: Header=BB2_3 Depth=1
	jmp	LBB2_15
LBB2_15:                                ##   in Loop: Header=BB2_3 Depth=1
	jmp	LBB2_3
LBB2_16:
	jmp	LBB2_17
LBB2_17:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rcx
	cmpq	%rcx, %rax
	jne	LBB2_19
## %bb.18:
	addq	$272, %rsp              ## imm = 0x110
	popq	%rbp
	retq
LBB2_19:
	callq	___stack_chk_fail
	ud2
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"yes"

L_.str.1:                               ## @.str.1
	.asciz	"y"

L_.str.2:                               ## @.str.2
	.asciz	"no"

L_.str.3:                               ## @.str.3
	.asciz	"n"

L_.str.4:                               ## @.str.4
	.asciz	"Please enter \"yes\" or \"no\" > "

L_.str.5:                               ## @.str.5
	.asciz	"I give up! What animal is it? > "

L_.str.6:                               ## @.str.6
	.asciz	"I'm guessing: %s\n"

L_.str.7:                               ## @.str.7
	.asciz	"Am I right? > "

L_.str.8:                               ## @.str.8
	.asciz	"I win!\n"

L_.str.9:                               ## @.str.9
	.asciz	"\noops.  What animal were you thinking of? > "

L_.str.10:                              ## @.str.10
	.asciz	"Enter a yes/no question to distinguish between a %s and a %s > "

L_.str.11:                              ## @.str.11
	.asciz	"What is the answer for a %s (yes or no) > "

L_.str.12:                              ## @.str.12
	.asciz	"%s (yes/no) > "


.subsections_via_symbols
