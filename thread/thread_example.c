#include "riscv.h"
#include "uart.h"
#include "thread.h"
#include "assert.h"

static unsigned thread_count, thread_sum;

static void thread_code(void *arg) {
    unsigned *x = arg;

    // check tid
    unsigned tid = get_cur_thread()->tid;
    uart_printk(UART1, "in thread %p, with %x\n", tid, *x);

    // check yield.
    thread_yield();
    thread_count ++;
    thread_yield();
    thread_sum += *x;
    thread_yield();
    // check exit
    thread_exit(0);
}

int main(void) {
    // Simple UART example, reads characters from UART and echoes them back
    uart_init(UART1);

    uart_printk(UART1, "about to test summing of 25 threads\n");

    int n = 25;
    thread_sum = thread_count = 0;

    unsigned sum = 0;
    for(int i = 0; i < n; i++)  {
        int *x = kmalloc(sizeof *x);
        sum += *x = i;
        thread_fork(thread_code, x);
    }
    thread_start();

    // no more threads: check.
    uart_printk(UART1, "count = %d, sum=%d\n", thread_count, thread_sum);
    assert(thread_count == n);
    assert(thread_sum == sum);

    uart_printk(UART1, "SUCCESS!\n");
}
