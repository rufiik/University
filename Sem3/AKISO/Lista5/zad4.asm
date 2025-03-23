; nasm -f elf32 zad4.asm
; ld -m elf_i386 zad4.o -o zad4

%include 'funkcje1.asm'

section .bss
table resb 100000

section .data
table_size equ 100000

section .text
global _start
_start:
    mov ebx, table
    mov ecx, table_size
    mov al, 0
elementy:
    mov [ebx], al
    inc ebx
    loop elementy

    mov eax, 2
cmpandprint:
    mov ebx, table
    add ebx, eax
    cmp byte [ebx], 0
    jne increment
    push eax
    call iprintLF
    pop eax
    mov ebx, table
    add ebx, eax
notprime:
    add ebx, eax
    cmp ebx, table+table_size
    jnc increment
    mov byte [ebx], 1
    jmp notprime
increment:
    inc eax
    cmp eax, table_size
    jne cmpandprint

    call quit
