#include <string.h>
#include "riscv.h"

// Pre-defined memory locations for program initialization.
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

void _cstart() {
    // extern int __bss_start__, __bss_end__;
	void main();

    // custom_loader();

    // int* bss = &__bss_start__;
    // int* bss_end = &__bss_end__;
 
    // while( bss < bss_end )
    //     *bss++ = 0;

    // uart_init();
    // Copy initialized data from .sidata (Flash) to .data (RAM)
    memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
    // Clear the .bss RAM section.
    memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );

    main(); 
	// clean_reboot();
}
