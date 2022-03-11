#include <stdint.h>

// Simple 'busy loop' delay method.
void delay_cycles(uint32_t cyc) {
    uint32_t d_i;
    for ( d_i = 0; d_i < cyc; ++d_i ) {
        __asm__( "nop" );
    }
}
