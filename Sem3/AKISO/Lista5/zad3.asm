; nasm -f elf32 zad3.asm
; ld -m elf_i386 zad3.o -o zad3


section .bss
    buffer resb 12
    input resb 10

section .text
    global _start

_start:
    mov eax, 3
    mov ebx, 0
    mov ecx, input
    mov edx, 10
    int 0x80
    mov esi, input
    xor eax, eax
    xor ebx, ebx

convert_input:
    mov bl, byte [esi]
    cmp bl, 10
    je convert_done
    sub bl, '0'
    imul eax, eax, 10
    add eax, ebx
    inc esi
    jmp convert_input

convert_done:
    ; Konwersja liczby na szesnastkowy format
    mov edi, buffer + 11
    mov ecx, 8

convert_loop:
    mov ebx, eax
    and ebx, 0xF
    add bl, '0'                ; konwertuj na znak ASCII
    cmp bl, '9'
    jbe store_char
    add bl, 7                  ; jeśli wiekszy niz 9, dodaj 7, aby uzyskać 'A'-'F'

store_char:
    mov [edi], bl
    dec edi
    shr eax, 4                 ; przesuń liczbę w prawo o 4 bity
    loop convert_loop

    ; Dodaj prefiks "0x"
    mov byte [edi], 'x'
    dec edi
    mov byte [edi], '0'
  mov byte [buffer + 12], 10 ; dodaj '\n' na końcu bufora
    mov edx, buffer + 13
    sub edx, edi

    mov eax, 4                 ; syscall number (sys_write)
    mov ebx, 1
    lea ecx, [edi]
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80
