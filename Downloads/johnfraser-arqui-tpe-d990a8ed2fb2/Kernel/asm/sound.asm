GLOBAL _enable_sound
GLOBAL _disable_sound

section .text

_enable_sound:
	in al, 0x61
	or al, 3
	out 0x61, al
	ret

_disable_sound:
	in al, 0x61
	and al, 0xfc
	out 0x61, al
	ret
	
