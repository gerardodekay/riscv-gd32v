#ifndef __ASSERT_H__
#define __ASSERT_H__

#include "uart.h"

#define panic(msg, args...) do { 					                            \
    uart_init(UART0);                                                           \
	(uart_printk)(UART0, "PANIC:%s:%s:%d:" msg "\n", __FILE__, __FUNCTION__,    \
            __LINE__, ##args);                                                  \
	while(1);							                                        \
} while(0)

#ifndef assert
#define assert(bool) do { if((bool) == 0) panic("%s", #bool); } while(0)
#endif

#endif