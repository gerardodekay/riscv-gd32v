#include "riscv.h"

// end of all the code/data in a binary file.
extern char __heap_start__;

// track if initialized.
static int init_p;

static unsigned heap;
static unsigned init_heap;

static inline uint32_t max_u32(uint32_t x, uint32_t y) {
    return x > y ? x : y;
}
static inline unsigned is_aligned(unsigned x, unsigned n) {
    return (((x) & ((n) - 1)) == 0);
}
static inline unsigned is_aligned_ptr(void *ptr, unsigned n) {
    return is_aligned((unsigned)ptr, n);
}
static inline unsigned is_pow2(unsigned x) {
    return (x & -x) == x;
}
static inline unsigned roundup(unsigned x, unsigned n) {
    return (x+(n-1)) & (~(n-1));
}

/*
 * One-time initialization, called before kmalloc 
 * to setup heap. 
 */
static void kmalloc_init(void) {
    if(init_p)
        return;
    init_p = 1;

    // call kmalloc_init_set_start w/ right values.
    heap = roundup((unsigned)&__heap_start__, 4);
    init_heap = heap;
}

/*
 * Return a memory block of at least size <nbytes>
 *  - There is no free, so is trivial.
 *  - The returned pointer should always be 4-byte aligned.  
 */
void *kmalloc(unsigned nbytes) {
    kmalloc_init();
    nbytes = roundup(nbytes, 4);
    void *ret = (void *)heap;
    heap = heap + nbytes;
    memset(ret, 0, nbytes);
    return ret;
}

/*
 * Address of returned pointer should be a multiple of alignment. 
 */
void *kmalloc_aligned(unsigned nbytes, unsigned alignment) {
    kmalloc_init();
    if (!is_pow2(alignment)) {
        return NULL;
    }

    if(alignment <= 4)
        return kmalloc(nbytes);
    
    nbytes = roundup(nbytes, 4);
    heap = roundup(heap, alignment);
    void *ret = (void *)heap;
    heap = heap + nbytes;
    memset(ret, 0, nbytes);
    return ret;
}

/* 
 * Free all allocated memory: reset the heap pointer back to the beginning.
 */
void kfree_all(void) {
    heap = init_heap;
}
