#include <stdint.h>

#include "riscv.h"
#include "gpio.h"

/*
 * The onboard LEDs are connected to the LED cathodes, so pulling the pin high
 * turns the LED off, and low turns it on.
 */
#define LED_ON 0
#define LED_OFF 1
#define DELAY_CYCLES 500000

int main(void) {
    // Enable the GPIOA and GPIOC peripherals.
    gpio_init(GPIOA);
    gpio_init(GPIOC);

    // Configure pins A1, A2, and C13 as outputs.
    gpio_set_output(GPIOA, 1);
    gpio_set_output(GPIOA, 2);
    gpio_set_output(GPIOC, 13);

    // Turn all three LEDs off.
    gpio_write(GPIOA, 1, LED_OFF);
    gpio_write(GPIOA, 2, LED_OFF);
    gpio_write(GPIOC, 13, LED_OFF);

    // Cycle the RGB LED through a pattern of colors.
    while( 1 ) {
        // Green on (Green)
        gpio_write(GPIOA, 1, LED_ON);
        delay_cycles(DELAY_CYCLES);

        // Red on (Yellow)
        gpio_write(GPIOC, 13, LED_ON);
        delay_cycles(DELAY_CYCLES);

        // Blue on (White)
        gpio_write(GPIOA, 2, LED_ON);
        delay_cycles(DELAY_CYCLES);

        // Green off (Purple)
        gpio_write(GPIOA, 1, LED_OFF);
        delay_cycles( DELAY_CYCLES );

        // Red off (Blue)
        gpio_write(GPIOC, 13, LED_OFF);
        delay_cycles( DELAY_CYCLES );

        // Blue off (Off)
        gpio_write(GPIOA, 2, LED_OFF);
        delay_cycles( DELAY_CYCLES );
    }
    return 0;
}
