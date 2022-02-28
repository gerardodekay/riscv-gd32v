#ifndef __GPIO_H__
#define __GPIO_H__

// GD32VF103_Datasheet_Rev1.0 - p. 83-84
#define RCU_APB2EN 0x40021018

// Available GPIO ports
typedef enum 
{ 
    GPIOA = 0, 
    GPIOB, 
    GPIOC,
    GPIOD,
    GPIOE
} gpio_port;

void gpio_init(gpio_port port);

// set GPIO <port><pin> to input
void gpio_set_input(gpio_port port, unsigned pin);

// set GPIO <port><pin> to output
void gpio_set_output(gpio_port port, unsigned pin);

// read <port><pin>: 1 = high, 0 = low.
unsigned gpio_read(gpio_port port, unsigned pin);

// write <port><pin> = <val>, 1 = high, 0 = low
// <port><pin> must be in output mode, other pins will be unchanged
void gpio_write(gpio_port port, unsigned pin, unsigned v);

#endif
