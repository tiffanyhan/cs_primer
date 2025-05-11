section .text
global fib
fib:
	; rdi: n

	cmp rdi, 0x01 ; if n is 0 or 1, return n
	mov rax, rdi 
	jle _done

	dec rdi ;
	push rdi ; save n-1 before recursive calls clobber it

	call fib ; fib(n-1)

	pop rdi ; get n-1
	push rax ; stack now has fib(n-1)

	; fib(n-1) has now returned

	dec rdi ; rdi: n-2

	call fib ; fib(n-2)
	mov rsi, rax ; rsi: fib(n-2)

	pop rax ; rax: fib(n-1) 
	add rax, rsi ; rax: fib(n-1) + fib(n-2)
	
_done:
	ret
