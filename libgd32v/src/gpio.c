#include "riscv.h"
#include "gpio.h"
#include "bit-support.h"

// GD32VF103_User_Manual_EN_V1.0.pdf - p. 83-84
#define RCU_APB2EN 0x40021018

// GD32VF103_User_Manual_EN_V1.0.pdf - p. 113
static uint32_t *gpio_base_address[] = {
    (uint32_t *)0x40010800, // GPIOA
    (uint32_t *)0x40010C00, // GPIOB
    (uint32_t *)0x40011000, // GPIOC
};

void gpio_init(gpio_port port) {
    // Enable GPIO port's clock
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 84
    uint32_t value = GET32(RCU_APB2EN);
    value = bit_set(value, port + 2);
    PUT32(RCU_APB2EN, value);
}

void gpio_set_function(gpio_port port, unsigned pin, gpio_func_t function) {
    if (pin >= 16) {
        // Invalid pin number
        return;
    }

    volatile void *gpio_reg = gpio_base_address[port] + (pin / 8);
    uint32_t value = get32(gpio_reg);
    unsigned modeStart = (pin % 8) * 4;
    // Set mode and configuration bits for 2MHz push-pull ouput
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 114-115
    value = bits_set(value, modeStart, modeStart + 3, function);
    put32(gpio_reg, value);
}

void gpio_set_input(gpio_port port, unsigned pin) {
    if (pin >= 16) {
        // Invalid pin number
        return;
    }

    // Set OCTL to 0 to have the input pin pulled down
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 103
    volatile void *gpio_reg = gpio_base_address[port] + 3;
    uint32_t value = get32(gpio_reg);
    value = bit_clr(value, pin);
    put32(gpio_reg, value);

    gpio_set_function(port, pin, GPIO_FUNC_INPUT_W_PULL_UP_DWN);
}

void gpio_set_output(gpio_port port, unsigned pin) {
    gpio_set_function(port, pin, GPIO_FUNC_OUTPUT_LOW_SPEED);
}

unsigned gpio_read(gpio_port port, unsigned pin) {
    if (pin >= 16) {
        // Invalid pin number
        return 0;
    }

    volatile void *gpio_reg = gpio_base_address[port] + 2;
    // Use Port input status register to get the input value
    // GD32VF103_User_Manual_EN_V1.0.pdf - p. 117-118
    uint32_t value = get32(gpio_reg);
    value = value >> pin;
    unsigned v = value & 0x1;

    return v;
}

void gpio_write(gpio_port port, unsigned pin, unsigned v) {
    if (pin >= 16) {
        // Invalid pin number
        return;
    }

    volatile void *gpio_reg = gpio_base_address[port] + 4;
    uint32_t value = 0;
    // Use Port bit operate register to set or clear the pin
     // GD32VF103_User_Manual_EN_V1.0.pdf - p. 117-118
    value = bit_set(value, (v) ? pin : pin + 16);
    put32(gpio_reg, value);
}
