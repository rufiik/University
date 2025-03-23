; nasm -f elf zad2.asm
; ld -m elf_i386 zad2.o -o zad2
%include 'funkcje1.asm'

section .bss
table resd 9  ; Zmieniono na resd, aby przechowywać 9 liczb całkowitych (4 bajty każda)

section .data
table_size equ 9
msg    db  "Your input:", 0h
msg2   db  "Za malo argumentow", 0h
msg3   db  "Suma elementow:", 0h
msg4   db  "Suma na glownej przekatnej:", 0h

section .text
global _start
_start:
    pop     ecx
    pop     edx
    sub     ecx, 1
    mov     edx, 0
    cmp     ecx, table_size
    jne     error

    mov     ebx, table
elementy:
    cmp     ecx, 0h
    jz      noMoreArgs
    pop     eax
    call    atoi
    mov     [ebx], eax
    add     ebx, 4
    dec     ecx
    jmp     elementy
noMoreArgs:
    mov     ecx, table_size
    mov     ebx, table
sumArgs:
    cmp     ecx, 0h
    jz      printResult
    mov     eax, [ebx]
    add     edx, eax
    add     ebx, 4
    dec     ecx
    jmp     sumArgs

printResult:
    mov     eax,msg3
    call    sprint
    mov     eax, edx
    call    iprintLF

    mov     edx, 0
    mov     ecx, 3
    mov     ebx, table
sumDiagonal:
    cmp     ecx, 0h
    jz      printDiagonal
    mov     eax, [ebx]
    add     edx, eax
    add     ebx, 16         ; (4 bajty na element * 4)
    dec     ecx
    jmp     sumDiagonal
printDiagonal:
    mov     eax,msg4
    call    sprint
    mov     eax, edx
    call    iprintLF
    call    quit

error:
    mov     eax, msg2
    call    sprintLF
    call    quit
