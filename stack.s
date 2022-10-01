	.file	"stack.cpp"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"%ld"
.LC1:
	.string	"&stack"
.LC2:
	.string	"stack.birth"
.LC3:
	.string	"int main()"
.LC4:
	.string	"stack.cpp"
.LC6:
	.string	"--%lg--\n"
.LC8:
	.string	"-%lg-\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 112
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	mov	QWORD PTR -112[rbp], 0
	lea	rax, -112[rbp]
	mov	rsi, rax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	__isoc99_scanf@PLT
	mov	QWORD PTR -80[rbp], 0
	mov	QWORD PTR -72[rbp], 0
	mov	QWORD PTR -64[rbp], 0
	mov	QWORD PTR -56[rbp], 0
	mov	QWORD PTR -48[rbp], 0
	mov	QWORD PTR -40[rbp], 0
	mov	QWORD PTR -32[rbp], 0
	mov	QWORD PTR -24[rbp], 0
	mov	QWORD PTR -16[rbp], 0
	lea	rdx, .LC1[rip+1]
	mov	rax, QWORD PTR -112[rbp]
	mov	ecx, eax
	lea	rax, -80[rbp]
	mov	esi, ecx
	mov	rdi, rax
	call	_Z17stack_constructorP11StructStackiPKc@PLT
	test	eax, eax
	setne	al
	test	al, al
	je	.L2
	lea	rax, -80[rbp]
	mov	rdi, rax
	call	_Z16stack_destructorP11StructStack@PLT
	mov	eax, 1
	jmp	.L6
.L2:
	mov	rax, QWORD PTR -32[rbp]
	lea	r8, .LC2[rip]
	mov	ecx, 16
	lea	rdx, .LC3[rip]
	lea	rsi, .LC4[rip]
	mov	rdi, rax
	call	_Z15initialize_infoP10StructInfoPKcS2_iS2_@PLT
	movsd	xmm0, QWORD PTR .LC5[rip]
	movsd	QWORD PTR -88[rbp], xmm0
	mov	rax, QWORD PTR -88[rbp]
	movq	xmm0, rax
	lea	rax, .LC6[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	mov	rdx, QWORD PTR -88[rbp]
	lea	rax, -80[rbp]
	movq	xmm0, rdx
	mov	rdi, rax
	call	_Z13push_in_stackP11StructStackd@PLT
	pxor	xmm0, xmm0
	movsd	QWORD PTR -104[rbp], xmm0
	lea	rdx, -104[rbp]
	lea	rax, -80[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_Z14pop_from_stackP11StructStackPd@PLT
	test	eax, eax
	setne	al
	test	al, al
	je	.L4
	lea	rax, -80[rbp]
	mov	rdi, rax
	call	_Z16stack_destructorP11StructStack@PLT
	mov	eax, 1
	jmp	.L6
.L4:
	mov	rax, QWORD PTR -104[rbp]
	movq	xmm0, rax
	lea	rax, .LC8[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	pxor	xmm0, xmm0
	movsd	QWORD PTR -96[rbp], xmm0
	lea	rdx, -96[rbp]
	lea	rax, -80[rbp]
	mov	rsi, rdx
	mov	rdi, rax
	call	_Z15peek_from_stackP11StructStackPd@PLT
	test	eax, eax
	setne	al
	test	al, al
	je	.L5
	lea	rax, -80[rbp]
	mov	rdi, rax
	call	_Z16stack_destructorP11StructStack@PLT
	mov	eax, 1
	jmp	.L6
.L5:
	mov	rax, QWORD PTR -96[rbp]
	movq	xmm0, rax
	lea	rax, .LC8[rip]
	mov	rdi, rax
	mov	eax, 1
	call	printf@PLT
	lea	rax, -80[rbp]
	mov	rdi, rax
	call	_Z16stack_destructorP11StructStack@PLT
	mov	eax, 0
.L6:
	mov	rdx, QWORD PTR -8[rbp]
	sub	rdx, QWORD PTR fs:40
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC5:
	.long	0
	.long	1091047432
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
