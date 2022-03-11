#ifndef __UART_H__
#define __UART_H__

typedef enum 
{ 
    UART0 = 0, 
    UART1, 
    UART2,
} uart_index;

void uart_init(uart_index uart);

void uart_disable(uart_index uart);

int uart_can_getc(uart_index uart);

char uart_getc(uart_index uart);

int uart_can_putc(uart_index uart);

void uart_putc(uart_index uart, char c);

int uart_printk(uart_index uart, const char *fmt, ...);

#endif
