%define TARGET_SEEN 0x03ffffff

section .text
global pangram

pangram:
	mov rax, 0 ; init rax as bitset

.loop:
	movzx rsi, byte [rdi]; deference the pointer and read the 1st byte
	cmp rsi, 0 ; check if null-terminating byte
	je .end ; break from loop
	inc rdi ; increment pointer by 1

	; check btwn A-Z or a-z

	cmp rsi, 'A' ; if < A, continue to next char
	jl .loop

	cmp rsi, 'z' ; if > z, continue to next char
	jg .loop

	cmp rsi, 'a' ; if >= a (a-z)
	jge .normalize ; char is a-z
	jmp .validate_upper ; char is either A-Z or non-alpha

.normalize:
	sub rsi, 32; convert lowcase to upcase (a-z becomes A-Z)

.validate_upper:
	cmp rsi, 'Z' ; if <= Z (btwn A-Z)
	jle .is_upper ; 
	jmp .loop ; not alpha, continue to next char

.is_upper:
	mov rdx, 1 ; init rdx as mask
	sub rsi, 'A' ; map rsi from ascii code to index

	mov rcx, rsi ; left shift opcode expects shift amount to be in CL register
	shl rdx, cl ; left shift mask by index times
	or rax, rdx ; set the index bit in the bitset using |= with mask

	jmp .loop
	
.end:
	cmp rax, TARGET_SEEN ; 26 lower bits set, all chars seen
	je .is_pangram

.not_pangram:
	mov rax, 0
	ret

.is_pangram:
	mov rax, 1
	ret
