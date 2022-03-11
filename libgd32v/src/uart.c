#include "riscv.h"
#include "uart.h"
#include "bit-support.h"
#include "gpio.h"

// GD32VF103_User_Manual_EN_V1.0.pdf - p. 83-84
#define RCU_APB2EN 0x40021018
#define RCU_APB1EN 0x4002101C

// GD32VF103_User_Manual_EN_V1.0.pdf - p. 344
static uint32_t *uart_base_address[] = {
    (uint32_t *)0x40013800, // UART0
    (uint32_t *)0x40004400, // UART1
    (uint32_t *)0x40004800, // UART2
};

void uart_init(uart_index uart) {
    uint32_t value;
    gpio_port gpio;

    // Enable UART's GPIO
    // Pins defined at GD32VF103_Datasheet_Rev1.0.pdf - p. 30-32
    switch (uart) {
        case UART0:
        case UART1:
            gpio = GPIOA;
            break;
        case UART2:
            gpio = GPIOB;
            break;
        default:
            // Invalid
            return;
    }
    gpio_init(gpio);

    // Enable UART's clock
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 84-86
    if (uart == UART0) {
        value = GET32(RCU_APB2EN);
        value = bit_set(value, 14);
        PUT32(RCU_APB2EN, value);
    }
    else {
        value = GET32(RCU_APB1EN);
        value = bit_set(value, uart + 16);
        PUT32(RCU_APB1EN, value);
    }

    switch (uart) {
        case UART0:
            gpio_set_function(gpio, 9, GPIO_FUNC_AFIO_W_PUSH_PULL);
            gpio_set_function(gpio, 10, GPIO_FUNC_AFIO_W_PUSH_PULL);
        case UART1:
            gpio_set_function(gpio, 2, GPIO_FUNC_AFIO_W_PUSH_PULL);
            gpio_set_function(gpio, 3, GPIO_FUNC_AFIO_W_PUSH_PULL);
            break;
        case UART2:
            gpio_set_function(gpio, 10, GPIO_FUNC_AFIO_W_PUSH_PULL);
            gpio_set_function(gpio, 11, GPIO_FUNC_AFIO_W_PUSH_PULL);
            break;
        default:
            // Invalid
            return;
    }

    // Set USART_CTL0 to disable UART and configure word length to 8 bits
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 347-348
    volatile void *uart_reg = uart_base_address[uart] + 3;
    value = 0;
    put32(uart_reg, value);
    
    // Set USART_CTL1 to configure to 1 stop bit
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 349-350
    uart_reg = uart_base_address[uart] + 4;
    value = 0;
    put32(uart_reg, value);

    // Set USART_CTL2 to disable DMA
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 350-352
    uart_reg = uart_base_address[uart] + 5;
    value = 0;
    put32(uart_reg, value);

    // Set the baud rate to 115200 using USART_BAUD.
    // Formula at GD32VF103_User_Manual_EN_V1.0.pdf - p. 331
    // PCLK frequency is set to 8MHz by default
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 346-347
    uart_reg = uart_base_address[uart] + 2;
    put32(uart_reg, 0x45);

    // Set the TEN & REN bits in USART_CTL0.
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 347-348
    uart_reg = uart_base_address[uart] + 3;
    value = bits_set(value, 2, 3, 0b11);;
    put32(uart_reg, value);

    // Set the UEN bit in USART_CTL0 to enable the USART
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 347-348
    value = bit_set(value, 13);
    put32(uart_reg, value);
}

void uart_disable(uart_index uart) {
    // Check TC is asserted before disabling the UART
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 344-345
    volatile void *uart_reg = uart_base_address[uart];
    while (!bit_is_on(get32(uart_reg), 5));

    // Clear the UEN bit in USART_CTL0 to disable the USART
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 347-348
    uart_reg = uart_base_address[uart] + 3;
    uint32_t value = 0;
    value = get32(uart_reg);
    value = bit_clr(value, 13);
    put32(uart_reg, value);
}

// 1 = at least one byte on rx queue, 0 otherwise
int uart_can_getc(uart_index uart) {
    // Check if RBNE is asserted in USART_STAT
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 344-345
    volatile void *uart_reg = uart_base_address[uart];
    return bit_is_on(get32(uart_reg), 5);
}

// returns one byte from the rx queue, if needed
// blocks until there is one.
char uart_getc(uart_index uart) {
    while (!uart_can_getc(uart));

    // Read the data from the USART_DATA register.
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 346
    volatile void *uart_reg = uart_base_address[uart] + 1;
    return (get32(uart_reg));
}

// 1 = space to put at least one byte, 0 otherwise.
int uart_can_putc(uart_index uart) {
    // Check if TBE is asserted in USART_STAT
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 344-345
    volatile void *uart_reg = uart_base_address[uart];
    return bit_is_on(get32(uart_reg), 7);
}

// put one byte on the tx qqueue, if needed, blocks
// until TX has space.
void uart_putc(uart_index uart, char c) {
    while (!uart_can_putc(uart));

    // Write the data to the USART_DATA register.
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 346
    volatile void *uart_reg = uart_base_address[uart] + 1;
    put32(uart_reg, (uint32_t)c);
}
