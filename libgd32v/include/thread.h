#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdint.h>

// Stack size for each thread in bytes
#define THREAD_STACK_SIZE (1024)
#define THREAD_MAXSTACK (THREAD_STACK_SIZE / 4)

typedef struct thread {
    // we place stack at beginning to enforce its alignment
    uint32_t stack[THREAD_MAXSTACK];
    uint32_t *saved_sp;

    struct thread *next;
    uint32_t tid;
} thread_t;


thread_t *get_cur_thread(void);

thread_t *thread_fork(void (*code)(void *arg), void *arg);

void thread_exit(int exitcode);

void thread_yield(void);

void thread_start(void);

#endif