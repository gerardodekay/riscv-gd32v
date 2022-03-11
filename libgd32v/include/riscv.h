#ifndef __RISCV_H__
#define __RISCV_H__

#include <stdint.h>
#include <stddef.h>

// used to write device memory: do not use a raw store.
// writes the 32-bit value <v> to address <addr>:   
//     *(uint32_t *)addr = v;
void put32(volatile void *addr, uint32_t v);
// same, but takes <addr> as a uint32_t
void PUT32(uint32_t addr, uint32_t v);

// used to read device memory: do not use a raw dereference!
//
// returns the 32-bit value at <addr>:  
//   return *(uint32_t *)addr
uint32_t get32(const volatile void *addr);
// same but takes <addr> as a uint32_t
uint32_t GET32(uint32_t addr);

int isdigit(int c);

char *strcpy(char * s1, const char * s2);

int strcmp(const char *a, const char *b);

int strncmp(const char* _s1, const char* _s2, size_t n);

char *strcat (char *dest, const char *src);

#endif