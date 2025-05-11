section .text
global fib
fib:
	; rdi: n

	cmp rdi, 0x00
	je _return_0

	cmp rdi, 0x02
	jle _return_1

	push rdi ; save n before decrementing for recursive calls
	dec rdi ;

	call fib ; fib(n-1)

	pop rdi ; get n
	push rax ; stack now has fib(n-1)

	; fib(n-1) has now returned

	sub rdi, 2 ; rdi: n-2

	call fib ; fib(n-2)
	mov rsi, rax ; rsi: fib(n-2)

	pop rax ; rax: fib(n-1) 
	add rax, rsi ; rax: fib(n-1) + fib(n-2)
	
	ret

_return_0:
	mov rax, 0x00
	ret

_return_1:
	mov rax, 0x01
	ret
