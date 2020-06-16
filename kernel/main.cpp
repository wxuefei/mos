#include "lib/string.h"
#include "interrupt.h"
#include "gdt.h"
#include "memory.h"

int start_kernel();

extern "C" void _start() {
   start_kernel();
}

int start_kernel() {
   gdt_init();
   idt_init();
   memory_init();
   int i = *(int*)0x903;
   printk_hex(i);
   // int j = 1 / 0;
   printk("this is mos\n");
   // put_int(9);
   // put_char('\n');
   // put_int(0x00021a3f);
   // put_char('\n');
   // put_int(0x12345678);
   // put_char('\n');
   // put_int(0x00000000);
   while(1);
   return 0;
}
