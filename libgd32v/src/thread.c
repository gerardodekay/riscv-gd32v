#include "riscv.h"
#include "thread.h"
#include "assert.h"

void ctx_switch(uint32_t **old_sp_save, const uint32_t *new_sp);

#define E thread_t
#include "Q.h"

static Q_t runq, freeq;
static thread_t *cur_thread; // current running thread.
static thread_t *scheduler_thread; // first scheduler thread.

static unsigned tid = 1; // monotonically increasing thread id

/*
 * simplistic pool of thread blocks: used to make alloc/free faster (plus,
 * our kmalloc doesn't have free.
 */

// total number of thread blocks we have allocated.
static unsigned nalloced = 0;

static thread_t *th_alloc(void) {
    // keep a cache of freed thread blocks, call kmalloc if run out.
    thread_t *t = Q_pop(&freeq);

    if(!t) {
        t = kmalloc_aligned(sizeof *t, 16);
        nalloced++;
    }
#   define is_aligned(_p,_n) (((unsigned)(_p))%(_n) == 0)
    assert(is_aligned(&t->stack[0], 16));
    t->tid = tid++;
    return t;
}

static void th_free(thread_t *th) {
    // push on the front in case helps with caching.
    Q_push(&freeq, th);
}

/*
 * stack offsets for each saved register. 
 */
enum {
    S0_OFFSET = 0,
    S1_OFFSET,
    S2_OFFSET,
    S3_OFFSET,
    S4_OFFSET,
    S5_OFFSET,
    S6_OFFSET,
    S7_OFFSET,
    S8_OFFSET,
    S9_OFFSET,
    S10_OFFSET,
    RA_OFFSET
};

/*
 * return pointer to the current thread. 
 */
thread_t *get_cur_thread(void) {
    return cur_thread;
}

/*
 * create a new thread. 
 */
thread_t *thread_fork(void (*code)(void *arg), void *arg) {
    thread_t *t = th_alloc();

    // write this so that it calls code,arg.
    void init_trampoline(void);

    /*
     * must do the "brain surgery" (k.thompson) to set up the stack
     * so that when we context switch into it, the code will be
     * able to call code(arg).
     *
     *  1. write the stack pointer with the right value.
     *  2. store arg and code into two of the saved registers.
     *  3. store the address of init_trampoline into the lr
     *     position so context switching will jump there.
     */

    t->saved_sp = &t->stack[THREAD_MAXSTACK - 12];
    assert(is_aligned(t->saved_sp, 16));
    t->saved_sp[S0_OFFSET] = (uint32_t)code;
    t->saved_sp[S1_OFFSET] = (uint32_t)arg;
    t->saved_sp[RA_OFFSET] = (uint32_t)init_trampoline;

    Q_append(&runq, t);
    return t;
}

/*
 * exit current thread.  
 */
void thread_exit(int exitcode) {
    // when you switch back to the scheduler thread:
    if (Q_empty(&runq)) {
        thread_t *prev_thread = cur_thread;
        cur_thread = scheduler_thread;
        uint32_t *sp = prev_thread->saved_sp;
        th_free(prev_thread);
        ctx_switch(&sp, cur_thread->saved_sp);
        
    }
    else {
        thread_t *prev_thread = cur_thread;
        cur_thread = Q_pop(&runq);
        uint32_t *sp = prev_thread->saved_sp;
        th_free(prev_thread);
        ctx_switch(&sp, cur_thread->saved_sp);
    }
}

/*
 * yield the current thread.  
 */
void thread_yield(void) {
    thread_t *old = cur_thread;

    thread_t *t;
    if(!(t = Q_pop(&runq)))
        return;

    Q_append(&runq, old);
    cur_thread = t;
    ctx_switch(&old->saved_sp, cur_thread->saved_sp);
}

/*
 * starts the thread system.  
 */
void thread_start(void) {
    scheduler_thread = th_alloc();
    scheduler_thread->saved_sp = &scheduler_thread->stack[THREAD_MAXSTACK - 12];
    cur_thread = scheduler_thread;

    thread_t *prev_thread;
    // no other runnable thread: return.
    if (!Q_empty(&runq)) {
        prev_thread = cur_thread;
        cur_thread = Q_pop(&runq);

        ctx_switch(&prev_thread->saved_sp, cur_thread->saved_sp);
    }
}
