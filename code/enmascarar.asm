section .data
    num dw 20

section .text

global enmascarar_asm

enmascarar_asm:

    push ebp
    mov ebp,esp
    xor eax,eax
    xor ecx,ecx

    mov eax,[ebp+8]     ;Almacena la direccion del buffer1
    mov ebx,[ebp+16]    ;Almacena la direccion de la mascara
    mov ecx,[ebp+20]    ;Almacena el size
    
    jmp loop

loop:

    cmp ecx,0           ;Revisar que queden pixeles por recorrer
    je end              ;Sino terminar la funcion

    movq mm0,qword[eax] ;Guardar en el registro mm0 los primeros 8 caracteres de eax
    movq mm1,qword[ebx] ;Guardar en el registro mm1 los primeros 8 caracteres de ebx
    pxor mm0,mm1        ;Hace un xor entre los bytes de la img1 y la mascara
    movq [eax],mm0      ;Mover al registro eax los caracteres modificados
  
    add eax,8           ;mover eax 8 bytes
    add ebx,8
    sub ecx,8           ;decremento ecx en 8. (size/8)
    jmp loop

end:
    pop ebp
    ret