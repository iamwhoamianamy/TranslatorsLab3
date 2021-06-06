.386
.model FLAT, C

.data
	a		dd	5
	b		dd	5
	const_5	dd	5
	const_3	dd	3

.code
main proc
	fild	a
	fild	const_5
	fcomp
	fstsw	ax
	sahf
	jne m1
	fild	b
	fild	const_3
	fcomp
	fstsw	ax
	sahf
	jne m2
	fild	a
	fistp	b
	jmp m3
m2:
	fild	const_3
	fistp	b
m3:
	jmp m4
m1:
	fild	b
	fistp	a
m4:
	mov		eax, 0
	ret
main endp

end main