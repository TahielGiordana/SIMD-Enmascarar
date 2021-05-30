section .data
    num dq 0

section .text

global enmascarar_asm

enmascarar_asm:

    push ebp
    mov ebp,esp
    xor eax,eax
    xor ecx,ecx

    mov eax,[ebp+8]     ;Almacena la direccion del buffer1
    mov ebx,[ebp+12]    ;Almacena la direccion del buffer2
    mov edx,[ebp+16]    ;Almacena la direccion de la mascara
    mov ecx,[ebp+20]    ;Almacena el size
    
    jmp loop

loop:

    cmp ecx,0           ;Revisar que queden pixeles por recorrer
    jl end              ;Sino terminar la funcion

    movq mm0,qword[eax] ;Guardar en el registro mm0 los primeros 8 caracteres del buffer1
    movq mm1,qword[ebx] ;Guardar en el registro mm1 los primeros 8 caracteres del buffer2
    movq mm2,qword[edx] ;Guardar en el registro mm1 los primeros 8 caracteres de la mascara

    pand mm1,mm2

    pandn mm2,mm0
    por mm1,mm2

    movq [eax],mm1      ;Mover al registro eax los caracteres modificados
  
    add eax,8           ;mover eax 8 bytes
    add ebx,8
    add edx,8
    sub ecx,8           ;decremento ecx en 8. (size/8)
    jmp loop

end:
    pop ebp
    ret