
SECTION .text
global ignore_int
ignore_int:
   cld
   push rax
   push rbx
   push rcx
   push rdx
   push rbp
   push rdi
   push rsi

   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   
loop:
    jmp loop
