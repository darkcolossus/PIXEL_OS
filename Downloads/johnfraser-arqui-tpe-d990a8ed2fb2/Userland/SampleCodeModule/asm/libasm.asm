GLOBAL runSyscall
GLOBAL _beep

section .text

runSyscall:
	int 80h
	ret
