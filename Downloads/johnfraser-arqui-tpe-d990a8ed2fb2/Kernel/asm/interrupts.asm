GLOBAL kEnableInterrupts
GLOBAL kDisableInterrupts
GLOBAL kSetHandler
GLOBAL kGetIDTR

GLOBAL keyboardInterruptHandler
GLOBAL syscallInterruptHandler
GLOBAL TTInterruptHandler

EXTERN keyboardHandler
EXTERN syscallHandler
EXTERN TTHandler

align 16
%macro pusha 0
		push		rax
		push		rbx
		push		rcx
		push		rdx
		push		rbp
		push		rdi
		push		rsi
		push		r8
		push		r9
		push		r10
		push		r11
		push		r12
		push		r13
		push		r14
		push		r15
		push		fs
		push		gs
%endmacro

align 16
%macro popa 0
		pop			gs
		pop			fs
		pop			r15
		pop			r14
		pop			r13
		pop			r12
		pop			r11
		pop			r10
		pop			r9
		pop			r8
		pop			rsi
		pop			rdi
		pop			rbp
		pop			rdx
		pop			rcx
		pop			rbx
		pop			rax
%endmacro

section .text




kEnableInterrupts:
	sti
	ret

kDisableInterrupts:
	cli
	ret

kGetIDTR:
	sidt [rdi]
	ret

kSetHandler:

	mov rax, rsi

	shl rdi, 4			; quickly multiply rdi by 16 to go to the corresponding place in the table
	add rdi, rdx		; adds the base address of the idt table to the place in the table
	stosw						; store the low word (15..0)
	shr rax, 16
	add rdi, 4			; skip the gate marker
	stosw						; store the high word (31..16)
	shr rax, 16
	stosd						; store the high dword (63..32)

	ret




;------------;
;  Handlers  ;
;------------;
align 16
TTInterruptHandler:
	cli
	pusha
	mov rdi,	rsp
	call TTHandler
	cmp rax,0
	je .dero
	mov rsp,rax
	.dero:
	mov 	al, 0x20
	out 	0x20, al
	popa
	sti
	iretq

keyboardInterruptHandler:
	cli
	call    keyboardHandler
	mov     al, 0x20
	out     0x20, al
	sti
  iretq

syscallInterruptHandler:
	cli
	call 	syscallHandler
	mov     al, 0x20
	out     0x20, al
	sti
	iretq
