#ifndef __GPIO_H__
#define __GPIO_H__

// available GPIO ports
typedef enum 
{ 
    GPIOA = 0, 
    GPIOB, 
    GPIOC
} gpio_port;

// different functions we can set GPIO pins to.
// TODO - define all possible functions
typedef enum {
    GPIO_FUNC_OUTPUT_LOW_SPEED = 0b0010,
    GPIO_FUNC_INPUT_W_PULL_UP_DWN = 0b1000,
    GPIO_FUNC_AFIO_W_PUSH_PULL = 0b1011,
} gpio_func_t;

// initialize the GPIO <port>
void gpio_init(gpio_port port);

// set GPIO <port><pin> to function defined in gpio_func_t
void gpio_set_function(gpio_port port, unsigned pin, gpio_func_t function);

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
