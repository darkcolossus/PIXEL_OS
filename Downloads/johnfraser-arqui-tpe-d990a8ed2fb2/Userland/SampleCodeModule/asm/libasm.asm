GLOBAL runSyscall
GLOBAL _beep

section .text

runSyscall:


	push rbp
	mov rbp,rsp
	pushfq

	xor rax,rax
	mov rax, [rbp+16]

	int 80h

	popfq
	mov rsp,rbp
	pop rbp


	ret
