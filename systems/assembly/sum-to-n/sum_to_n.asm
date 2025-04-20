section .text
global sum_to_n

sum_to_n:
	mov rax, 0x00

_check:
	cmp rdi, 0x00
	je _done

	add rax, rdi
	dec rdi
	jmp _check

_done:
	ret
