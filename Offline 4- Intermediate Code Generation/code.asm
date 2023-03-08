.MODEL SMALL
.STACK 100h
.DATA
	CR EQU 0DH
	LF EQU 0AH
	number DB "00000$"
.CODE

f  PROC
	PUSH BP
	MOV BP,SP
	SUB SP,2
	MOV CX,5
	MOV -2[BP],CX
	;======WHILE loop start======
L1:
	MOV CX,-2[BP]
	PUSH CX
	MOV CX,0
	POP AX
	CMP AX,CX
	JG L2
	MOV CX,0
	JMP L3
L2:
	MOV CX,1
L3:
	JCXZ L4 ;break while loop
	MOV CX,4[BP]
	INC 4[BP]
	MOV CX,-2[BP]
	MOV AX,CX
	SUB AX,1
	MOV -2[BP],AX
	JMP L1
L4:
	;======WHILE loop end======
	;======RETURN STATEMENT start======
	MOV CX,3
	PUSH CX
	MOV CX,4[BP]
	POP AX
	IMUL CX
	MOV CX,AX
	PUSH CX
	MOV CX,7
	POP AX
	SUB AX,CX
	MOV CX,AX
	MOV DX,CX
	JMP f_end
	;======RETURN STATEMENT end======
	MOV CX,9
	MOV 4[BP],CX
f_end:
	ADD SP,2
	POP BP
	RET 2
f ENDP

g  PROC
	PUSH BP
	MOV BP,SP
	SUB SP,2
	SUB SP,2
	MOV CX,6[BP]
	PUSH CX
	CALL f
	MOV CX,DX
	ADD SP,0
	PUSH CX
	MOV CX,6[BP]
	POP AX
	ADD AX,CX
	MOV CX,AX
	PUSH CX
	MOV CX,4[BP]
	POP AX
	ADD AX,CX
	MOV CX,AX
	MOV -2[BP],CX
	;======for loop start======
	MOV CX,0
	MOV -4[BP],CX
L5:
	MOV CX,-4[BP]
	PUSH CX
	MOV CX,7
	POP AX
	CMP AX,CX
	JL L6
	MOV CX,0
	JMP L7
L6:
	MOV CX,1
L7:
	JCXZ L8 ;break for loop
	;======IF ELSE statement start======
	MOV CX,-4[BP]
	PUSH CX
	MOV CX,3
	POP AX
	CWD
	IDIV CX
	MOV CX,DX
	PUSH CX
	MOV CX,0
	POP AX
	CMP AX,CX
	JE L9
	MOV CX,0
	JMP L10
L9:
	MOV CX,1
L10:
	JCXZ L11 ;jump for false
	MOV CX,-2[BP]
	PUSH CX
	MOV CX,5
	POP AX
	ADD AX,CX
	MOV CX,AX
	MOV -2[BP],CX
	JMP L12 ;jump for true
L11:
	MOV CX,-2[BP]
	PUSH CX
	MOV CX,1
	POP AX
	SUB AX,CX
	MOV CX,AX
	MOV -2[BP],CX
L12:
	;======IF ELSE statement end======
	MOV CX,-4[BP]
	INC -4[BP]
	JMP L5
L8:
	;======for loop end======
	;======RETURN STATEMENT start======
	MOV CX,-2[BP]
	MOV DX,CX
	JMP g_end
	;======RETURN STATEMENT end======
g_end:
	ADD SP,4
	POP BP
	RET 4
g ENDP

main  PROC
	MOV AX,@DATA
	MOV DS,AX

	PUSH BP
	MOV BP,SP
	SUB SP,2
	SUB SP,2
	SUB SP,2
	MOV CX,1
	MOV -2[BP],CX
	MOV CX,2
	MOV -4[BP],CX
	MOV CX,-2[BP]
	PUSH CX
	MOV CX,-4[BP]
	PUSH CX
	CALL g
	MOV CX,DX
	ADD SP,0
	MOV -2[BP],CX
	MOV AX, -2[BP]
	CALL print_output
	CALL new_line
	;======for loop start======
	MOV CX,0
	MOV -6[BP],CX
L13:
	MOV CX,-6[BP]
	PUSH CX
	MOV CX,4
	POP AX
	CMP AX,CX
	JL L14
	MOV CX,0
	JMP L15
L14:
	MOV CX,1
L15:
	JCXZ L16 ;break for loop
	MOV CX,3
	MOV -2[BP],CX
	;======WHILE loop start======
L17:
	MOV CX,-2[BP]
	PUSH CX
	MOV CX,0
	POP AX
	CMP AX,CX
	JG L18
	MOV CX,0
	JMP L19
L18:
	MOV CX,1
L19:
	JCXZ L20 ;break while loop
	MOV CX,-4[BP]
	INC -4[BP]
	MOV CX,-2[BP]
	MOV AX,CX
	SUB AX,1
	MOV -2[BP],AX
	JMP L17
L20:
	;======WHILE loop end======
	MOV CX,-6[BP]
	INC -6[BP]
	JMP L13
L16:
	;======for loop end======
	MOV AX, -2[BP]
	CALL print_output
	CALL new_line
	MOV AX, -4[BP]
	CALL print_output
	CALL new_line
	MOV AX, -6[BP]
	CALL print_output
	CALL new_line
	;======RETURN STATEMENT start======
	MOV CX,0
	MOV DX,CX
	JMP main_end
	;======RETURN STATEMENT end======
main_end:
	ADD SP,6
	POP BP
	MOV AX,4CH
	INT 21H
main ENDP
new_line proc
	push ax
	push dx
	mov ah,2
	mov dl,cr
	int 21h
	mov ah,2
	mov dl,lf
	int 21h
	pop dx
	pop ax
	ret
new_line endp
print_output proc  ;print what is in ax
	push ax
	push bx
	push cx
	push dx
	push si
	lea si,number
	mov bx,10
	add si,4
	cmp ax,0
	jnge negate
	print:
	xor dx,dx
	div bx
	mov [si],dl
	add [si],'0'
	dec si
	cmp ax,0
	jne print
	inc si
	lea dx,si
	mov ah,9
	int 21h
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	ret
	negate:
	push ax
	mov ah,2
	mov dl,'-'
	int 21h
	pop ax
	neg ax
	jmp print
print_output endp
END main