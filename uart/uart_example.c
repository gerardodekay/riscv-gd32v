#include "riscv.h"
#include "bit-support.h"
#include "uart.h"

int main(void) {
    // Simple UART example, reads characters from UART and echoes them back
    uart_init(UART1);

    while (1) {
        char c = uart_getc(UART1);
        uart_printk(UART1, "Got %c\n", c);
    }
}