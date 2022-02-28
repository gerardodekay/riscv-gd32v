// Standard library includes.
#include <stdint.h>
#include <string.h>

#include "riscv.h"
#include "gpio.h"

int main( void ) {
    // Enable the GPIOB peripheral.
    gpio_init(GPIOB);

    // Configure pin B1 as output.
    gpio_set_output(GPIOB, 1);

    // Configure pin B4 as input.
    gpio_set_input(GPIOB, 9);

    // Set pin B1 on or off according to the value of pin B4
    while (1) {
        gpio_write(GPIOB, 1, gpio_read(GPIOB, 9));
    }
    return 0;
}
