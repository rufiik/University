; Kompilowanie nasm -f elf test.asm
; ld -m elf_i386 test.o -o test

%include 'funkcje1.asm'

SECTION .data
msg1        db 'Podaj liczbe calkowita: ', 0h
msg2        db 'Wynik to: ', 0h

SECTION .bss
input:      resb 255

SECTION .text
global _start

_start:
    mov eax, msg1
    call sprint
    mov edx, 255
    mov ecx, input
    mov ebx, 0
    mov eax, 3
    int 80h
    xor ebx, ebx
    mov ecx, input
SumDigits:
    mov al, [ecx]     ; bieżący znak
    cmp al, 0Ah
    je End
    sub al, '0'       ; ASCII na cyfrę
    add ebx, eax
    inc ecx
    jmp SumDigits
End:
    mov eax, msg2
    call sprint

    mov eax, ebx
    call iprintLF
    call quit
