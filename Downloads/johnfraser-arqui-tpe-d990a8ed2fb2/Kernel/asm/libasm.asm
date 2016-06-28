GLOBAL cpuVendor
GLOBAL kin
GLOBAL kout
GLOBAL _beep
GLOBAL cr0Read 
GLOBAL cr0Write
GLOBAL cr3Read
GLOBAL cr3Write	


section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;------------;
;  Port I/O  ;
;------------;

kin:
	mov	dx, di
	in	al, dx

	ret

kout:
	mov rax, 0x0
	mov rdx, 0x0

	mov ax, si
	and ax, 0xFF
	mov dx, di
	out dx, al

	ret

_beep:
	mov     al, 182
	out     43h, al
	mov     rax, 4560
	
	out     42h, al
	mov     al, ah
	out     42h, al 
	in      al, 61h 
	
	or      al, 00000011b
	out     61h, al
	mov     bx, 320
.loop1:
	mov     cx, 65535
.loop2:
	dec     cx
	jne     .loop2
	dec     bx
	jne     .loop1
	in      al, 61h
	
	and     al, 11111100b
	out     61h, al
	ret 

;------------;
;  Memory	 ;
;------------;

cr0Read:
	mov rax, cr0
	ret

cr0Write:
	mov cr0, rdi
	ret
	
cr3Read:
	mov rax, cr3
	ret

cr3Write:
	mov cr3, rdi
	ret
