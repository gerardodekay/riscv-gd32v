#include "riscv.h"
#include "uart.h"

int main(void) {
    // Test malloc
    uart_init(UART1);
    uart_printk(UART1, "About to allocate 1K buffer\n");
    void *mem = kmalloc(1024);
    uart_printk(UART1, "Allocated memory at addr = %x\n", mem);
}
