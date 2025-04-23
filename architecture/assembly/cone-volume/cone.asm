default rel

section .text
global volume

volume:
	; xmm0: radius, 1st float arg
	; xmm1: height, 2nd float arg

	mulss xmm0, xmm0 ; r^2
	divss xmm1, [three] ; h/3
	mulss xmm0, xmm1 ; (r^2) * (h/3)
	mulss xmm0, [pi] ; (r^2 * h/3) * pi

 	ret ; return value in xmm0

section .data
	pi dd 3.14159
	three dd 3.0
