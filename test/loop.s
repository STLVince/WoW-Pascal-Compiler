	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	movl	$0, s(%rip)
	movl	$1, i(%rip)
	cmpl	$10, i(%rip)
	jg	.LBB0_3
	.p2align	4, 0x90
.LBB0_2:
	movl	i(%rip), %eax
	addl	%eax, s(%rip)
	incl	%eax
	movl	%eax, i(%rip)
	cmpl	$10, i(%rip)
	jle	.LBB0_2
.LBB0_3:
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	s,@object
	.bss
	.globl	s
	.p2align	2
s:
	.long	0
	.size	s, 4

	.type	i,@object
	.globl	i
	.p2align	2
i:
	.long	0
	.size	i, 4


	.section	".note.GNU-stack","",@progbits
