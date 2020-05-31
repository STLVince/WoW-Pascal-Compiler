	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	movl	$2, a(%rip)
	movl	$4, i(%rip)
	movl	$4, j(%rip)
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

	.type	i,@object
	.globl	i
	.p2align	2
i:
	.long	0
	.size	i, 4

	.type	j,@object
	.globl	j
	.p2align	2
j:
	.long	0
	.size	j, 4


	.section	".note.GNU-stack","",@progbits
