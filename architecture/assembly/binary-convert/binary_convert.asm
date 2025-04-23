section .text
global binary_convert

binary_convert:
	xor rax, rax ; rax is used as acc for building up the int

.loop: ; iterating over input str from left to right
	movzx r10, byte [rdi] ; r10 holds the current char
	cmp r10, 0 ; reached a null-terminating byte
	je .end ; break from the loop
	inc rdi ; increment the pointer by 1 byte

	shl rax, 1 ; multiply acc by 2
	xor r11, r11 ; zero out r11 to hold 0 or 1
	cmp r10, 0x31 ; see if char is "0" or "1"
	sete r11b ; if char is "1", set r11 to 1
	add rax, r11 ; add 0 or 1 to rax
	jmp .loop

.end:
	ret ; return rax which holds the int we accumulated
