section .text
global index
index:
	; rdi: matrix
	; esi: rows
	; edx: cols
	; ecx: rowindex
	; r8d: colindex

	mov eax, ecx ; calculate index offset as ((rowIndex * cols) + colIndex)
	mul edx
	add eax, r8d ; eax now has index offset
	
	mov rax, [rdi + rax * 4] ; load memory address using matrix address + byte offset

	ret ; rax: ints returned in rax
