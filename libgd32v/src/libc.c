#include "riscv.h"

int isdigit(int c) { return (c >= '0' && c <= '9'); }

char *strcpy(char * s1, const char * s2) {
    register char *s = s1;
    while ( (*s++ = *s2++) != 0 );
    return s1;
}

int strcmp(const char *a, const char *b) {
        while (*a && *a == *b)
                ++a, ++b;
        return *a - *b;
}

int strncmp(const char* _s1, const char* _s2, size_t n) {
    const unsigned char *s1 = (void*)_s1, *s2 = (void*)_s2;
    while(n--) {
        if(*s1++!=*s2++)
            return s1[-1] - s2[-1];
    }
    return 0;
}

char *strcat (char *dest, const char *src) {
    char *s1 = dest;
    const char *s2 = src;
    char c;

    /* Find the end of the string.  */
    do
        c = *s1++;
    while (c != '\0');

    /* Make S1 point before the next character, so we can increment
       it while memory is read (wins on pipelined cpus).  */
    s1 -= 2;
    do {
        c = *s2++;
        *++s1 = c;
    } while (c != '\0');
    return dest;
}
