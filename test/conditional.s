	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	movl	$1, a(%rip)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB0_2
	movl	$1, s(%rip)
	retq
.LBB0_2:
	movl	$0, s(%rip)
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	a,@object
	.bss
	.globl	a
	.p2align	2
a:
	.long	0
	.size	a, 4

	.type	s,@object
	.globl	s
	.p2align	2
s:
	.long	0
	.size	s, 4


	.section	".note.GNU-stack","",@progbits
