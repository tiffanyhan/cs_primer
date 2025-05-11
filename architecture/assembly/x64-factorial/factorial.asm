section .text
global factorial

; n = 3
; rax set to 1
; call factorial(n-1)

; n = 2
; rax set to 1
; call factorial(n-1)

; n = 1
; rax set to 1
; call factorial(n-1)

; n = 2
; rax has 1 set by prev frame

; n = 3
; rax has 2 set by prev frame

factorial:
	; rdi: n

	mov rax, 0x01 ; TODO: save

	cmp rdi, 0x01 ; early return if n is 0 or 1
	jle _done ; assume no negative inputs will be passed

	push rdi ; push n before decrementing
	dec rdi ; decrement n for next fn call

	call factorial ; invoke factorial(n-1)

	; rax has return val of recursive call

	pop rdi ; restore n
	imul rax, rdi ; multiply n * (n-1)!

_done:
	ret ; rax now has n!
