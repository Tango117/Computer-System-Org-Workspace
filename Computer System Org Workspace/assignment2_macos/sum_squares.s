        # LEAVE THIS PART ALONE
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.globl	_sum_squares
	.p2align	4, 0x90

	# This is the definition of the sum_squares function.
	# See the declaration in sum_squares.h
	# The function takes one parameter:
	#   -- a 32-bit integer n in the %edi register
	# It returns the sum of squares of the numbers from 1 to n.
	# The return value, being 32 bits, must be placed in the %eax register.

_sum_squares:

	# In general, you can overwrite the 64-bit
	# registers %rax, %rcx, %rdx, %rsi, %rdi, %r8, %r9, %r10, %r11,
	# as well as their 32-bit halves, %eax, %ecx, %edx,
	# %esi, %edi, %r8d, %r9d, %r10d, %r11d as you like. These are
	# the "caller-saved" registers.

	# NOTE: In this assignment, you only need to use 32-bit
	# registers for the computation (i.e. %eax, %ecx, %edx, etc.)

	pushq	%rbp         # LEAVE THIS ALONE
	movq	%rsp, %rbp   # LEAVE THIS ALONE

	movl $0, %eax  # sum = 0, since sum is to be returned,
	                    #          use %eax to hold sum.

	movl $1, %ecx  # i = 1

/* THIS IS THE TOP OF THE LOOP, NEED A LABEL */
l1:
	cmpl %ecx, %edi  # compare i to n
	jl l2  # if i is greater than n, jump out of loop

	movl %ecx, %edx  # copy i into another register
	imul %ecx, %edx  # multiply that register by i to get i*i
	add %edx, %eax  # add that register to sum

	inc %ecx  # i++
	jmp l1  # jump to top of loop

/* THIS IS OUTSIDE THE LOOP, NEED A LABEL */
l2:
	                # the return value, sum, is already in %eax

	popq	%rbp    # LEAVE THIS ALONE
	retq            # LEAVE THIS ALONE
