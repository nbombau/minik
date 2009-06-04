GLOBAL  _read_msw,_lidt
GLOBAL  _lgdt
GLOBAL  _getgdt
GLOBAL  _int_08_hand, _int_80_hand, write, read, _int_09_hand_US,_int_09_hand_LAT
GLOBAL myin, myout, myinw, myoutw, myinl, myoutl
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL _DelCursor
GLOBAL switch_manual
GLOBAL _Fault
GLOBAL  ArmaStackFrame

EXTERN  int_08
EXTERN  int_80w
EXTERN  int_80r
EXTERN  int_09_US
EXTERN  int_09_LAT
EXTERN 	read_key
EXTERN 	send_eoi

;Excepciones
GLOBAL div0_hand
GLOBAL bounds_hand
GLOBAL opCode_hand
GLOBAL snoPresent_hand
GLOBAL ssf_hand
GLOBAL generalPfault_hand
GLOBAL pageFault_hand

GLOBAL  _leo_reloj


EXTERN  Div0
EXTERN  Bounds
EXTERN  opCode
EXTERN  SnoPresent
EXTERN  Stacksf
EXTERN  GpFault
EXTERN  PageFault

EXTERN  LoadESP
EXTERN  SaveESP
EXTERN  SiguienteProceso
EXTERN sleep
EXTERN printf
GLOBAL _StackFill
EXTERN Paginas
SECTION .data

msg db "Pase por aca",0

msg2 db "Termine los ciclos",0

pint db "%d\n",0

SECTION .text

_miwait:
	mov eax,0
primero:
	mov edx,0
segundo:inc edx
	cmp edx,7530h
	jnz segundo
	inc eax
	cmp eax,7530h
	jnz primero
	jp devuelta



_StackFill:
	push ebp
	mov ebp,esp

devuelta:
	mov ecx,0
ciclo32:	mov edx,0065h
	mov eax,0
	inc ecx
sigo456:   push eax
	inc eax
	cmp eax,edx
	jnz sigo456

	push 1
	call sleep
	pop ebx
    
	cmp ecx,000Ah
	jnz ciclo32

	push msg
	call Paginas
	pop ebx
	
	jp devuelta
	call _debug
	ret

;-----------------------------------------------
div0_hand:              ; Handler de excepxi�n "Divide by zero"
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax                  
    call Div0                 
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    popf
    popa
    pop es
    pop ds
    sti
    jmp $
    iret

bounds_hand:                    ; Handler de excepci�n "BOUND range exceeded"
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax
    call Bounds
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    popf
    popa
    pop es
    pop ds
    sti
    jmp $
    iret

opCode_hand:                            ; Handler de excepci�n "Invalid opcode"
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax
    call opCode
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    pop ax
    popa
    pop ax
    pop ax
    sti
    jmp $
    iret

snoPresent_hand:            ; Handler de excepci�n "Segment not present"
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax
    call SnoPresent
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    popf
    popa
    pop ax
    pop ax
    sti
    jmp $
    iret

ssf_hand:               ; Handler de excepci�n "Stack exception"
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax                  
    call Stacksf
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    popf
    popa
    pop ax
    pop ax
    sti
    jmp $
    iret

generalPfault_hand:         ; Handler de excepci�n "General protection exception"
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax
    call GpFault
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    popf
    popa
    pop ax
    pop ax
    sti
    jmp $
    iret


pageFault_hand:
    cli
    push ds
    push es             ; Se salvan los registros
    pusha               ; Carga de DS y ES con el valor del selector
    pushf
    mov ax, 10h         ; a utilizar.
    mov ds, ax
    mov es, ax                  
    call PageFault                 
    mov al,20h          ; Envio de EOI generico al PIC
    out 20h,al
    popf
    popa
    pop es
    pop ds
    sti
    jmp $
    iret
    
ArmaStackFrame:
    ; flags
    pushfd
    push ebp
    mov ebp, esp
    ; sp por parametros
    mov eax, [ebp+16]
    ; usar ese stack
    mov esp, eax
    ; funcion a ejecutar
    mov eax, [ebp+12]
    ; funcion cementerio
    mov edx, [ebp+20]
    ; push de 'la nueva ret'
    push edx
    ; seteo el bit de habilitar interrupciones
    mov ecx, 512
    ; push flags (solo me interesa habilitar int)
    push ecx
    ; para iret
    push cs
    push eax
    ; resto de los registros
    mov edi,ebp
    mov ebp,0
    pusha
    mov ebp,edi
    ; devolver el nuevo esp
    mov eax, esp
    ; y restaurar el original
    mov esp, ebp
    ; restaurar flags y ebp
    pop ebp
    popfd
    retn


_int_08_hand:
    cli
    pusha
    push esp
    ;push esp
    ;push pint
    ;call printf
    ;pop eax
    ;pop esp
    call SiguienteProceso
   
    pop esp
    mov esp, eax
    mov al, 20h
    out 20h, al
    popa
    sti
   iret


switch_manual:
    cli
    push ebp
    mov ebp, esp
    int 0x08
    mov esp, ebp
    pop ebp
    sti
    ret

_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR
	rol	ebx,16
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn

_leo_reloj:
	push	ecx ; Se salvan los registros
	push	edx
        push	ebx
	push	sp
	push	bp
	push   	si
	push	di
	push    ds
        push	es
	
	mov 	eax,00000000h
	mov 	al,00h
	out	70h,al
	in	al,71h
		
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	pop	sp
	pop	ebx
	pop	edx
	pop	ecx
	ret

_int_80_hand:
	push	ds
	push	es
	pusha

	push eax

	mov	ax, 10h
	mov	ds, ax
	mov	es, ax

	pop eax

	cmp al, 03h		;llama a myread
	je	myread
	cmp al, 04h		;llama a mywrite
	je	mywrite
	jmp	exit

myread:
	push	edx
	push	ecx
	push	ebx
	call    int_80r ; funcion implementada en c
	jmp	exit

mywrite:
	push	edx
	push	ecx
	push	ebx
	call    int_80w ; funcion implementada en c
	jmp	exit

exit:
	pop	ebx
	pop	ecx
	pop	edx

	mov	al, 20h
	out	20h, al
	popa
	pop	es
	pop	ds
	iret

write:

	push    ebp
    	mov     ebp, esp

    	mov     ebx, [ebp+8]    ; fd
    	mov     ecx, [ebp+12]   ; buffer
    	mov     edx, [ebp+16]   ; cant
		mov		al, 04h
    	int     080h

	mov     esp, ebp
	pop     ebp
	ret

read:

	push    ebp
    	mov     ebp, esp

    	mov     ebx, [ebp+8]    ; fd
    	mov     ecx, [ebp+12]   ; buffer
    	mov     edx, [ebp+16]   ; cant
		mov		al, 03h
    	int     080h
	mov     esp, ebp
	pop     ebp
	ret

myout:
	push EBP
	mov EBP, ESP
	mov EAX, [EBP+12]
	mov EDX, [EBP+8]
	out DX, AL
	pop EBP
	ret

myin:
	push EBP
	mov EBP, ESP
	mov EDX, [EBP+8]
	mov ECX, [EBP+12]
	in AL, DX
	mov [ECX], AL
	pop	EBP
	ret

myoutw:
	push EBP
	mov EBP, ESP
	mov EAX, [EBP+12]
	mov EDX, [EBP+8]
	out DX, AX
	pop EBP
	ret

myinw:
	push EBP
	mov EBP, ESP
	mov EDX, [EBP+8]
	mov ECX, [EBP+12]
	in AX, DX
	mov [ECX], AX
	pop	EBP
	ret

myoutl:
	push EBP
	mov EBP, ESP
	mov EAX, [EBP+12]
	mov EDX, [EBP+8]
	out DX, EAX
	pop EBP
	ret

myinl:
	push EBP
    mov EBP, ESP
	mov EDX, [EBP+8]
	mov ECX, [EBP+12]
	in EAX, DX
	mov [ECX], EAX
	pop	EBP
	ret

; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
;
_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn


; Handler del teclado LAT
_int_09_hand_LAT:			; Handler de INT 9 ( Teclado ).
	cli			; Deshabilito las interrupciones
	push    ds
	push    es
	pusha  			; Armo stack frame.

	mov     ax, 10h		; Carga en DS y ES con el valor del selector.
	mov     ds, ax		; a utilizar.
	mov     es, ax

	mov eax,0		; Lee del teclado el scancode y se lo pasa
	call read_key	; a la funcion int_09 a travez del stack.
	push eax

	call int_09_LAT

	; Envio de EOI generico al PIC
	call send_eoi
	pop eax

	popa			; Armo stack frame.
        pop     es
        pop     ds
	sti			; Vuelvo a habilitar las interrupciones
        iret





; Handler del teclado US
_int_09_hand_US:			; Handler de INT 9 ( Teclado ).
	cli			; Deshabilito las interrupciones
	push    ds
        push    es
        pusha  			; Armo stack frame.

	mov     ax, 10h		; Carga en DS y ES con el valor del selector.
        mov     ds, ax		; a utilizar.
        mov     es, ax

	mov eax,0		; Lee del teclado el scancode y se lo pasa
	call read_key		; a la funcion int_09 a traves del stack.
	push eax

	call int_09_US

	; Envio de EOI generico al PIC
	call send_eoi
	pop eax

	popa			; Armo stack frame.
        pop     es
        pop     ds
	sti			; Vuelvo a habilitar las interrupciones
        iret


_Lights:
    push	ebp
    mov		ebp, esp
    mov		al,[EBP+8]
    mov		ah,al

    cli                             ; Deshabilito interrupciones
    mov     al,0edh                 ; Cargo el Set/Reset Mode Indicator
    out     60h,al

l1: in      al,60h                  ; Espero el ACK del 8042
    cmp     al,0fah
    jnz     l1
    mov     al,ah                   ; Mando las nuevas luces
    out     60h,al
l2: in      al,60h                  ; Espero el ACK del 8042
    cmp     al,0fah
    jnz     l2
    sti                             ; habilito interrupciones de nuevo
	pop		ebp
    ret



_Fault:
 	INT 0h

[global read_cr0]
read_cr0:
	mov eax, cr0
	retn

[global write_cr0]
write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn

[global read_cr3]
read_cr3:
	mov eax, cr3
	retn

[global write_cr3]
write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn

