#include <stdarg.h>

#include "riscv.h"
#include "bit-support.h"
#include "uart.h"

static int uart_putchar(uart_index uart, int c) { uart_putc(uart, c); return c; }

static int uart_putk(uart_index uart, const char *p) {
    for(; *p; p++)
        uart_putchar(uart, *p);
    return 1;
}

static char* pad(char *p,  char *base, int width) {
    int len = p - base;
    // pad with leading spaces
    for(; len < width; len++)
        *p++ = ' ';
    *p = 0;
    return p;
}

// returns the initial value of dst.
static char* reverse(char *dst, int n, char *p, char *start) {
    // drop the lower chars if the string is too big.
    int len = p - start + 1; // include \0
    if(len > n)
        start += (len - n);

    char *s = dst;
    
    // p starts at end.
    for(p--; p >= start; p--)
        *dst++ = *p;
    *dst++ = 0;

    return s;
}

static char* emit(unsigned base, char *dst, int n, int val, int width,
        int signed_p) {
    // XXX: constrain width to be >= bit size: can change this.
    if(width > n)
        width = n - 1;

    char buf[64], *p = buf; 
    unsigned u = val;

    switch(base) { 
        case 10: 
        {
            unsigned neg_p = 0;
            if(val < 0 && signed_p) {
                neg_p = 1;
                val = -val;
                u = val;
            }
            do { 
                *p++ = "0123456789"[u % 10];
            } while(u /= 10); 
            if(neg_p)
                *p++ = '-';
            break;
        }
        case 16:
            u = val;
            do {
                *p++ = "0123456789abcdef"[u % 16];
            } while(u /= 16);
            *p++ = 'x';
            *p++ = '0';
                        break;
        // just a single char
        case 8:
            *p++ = val;
            break;
        case 2:
            u = val;
            do {
                *p++ = "01"[u % 2];
            } while(u /= 2);
            break;
        default:
            return(0);
    }
    // number string is reversed, so re-reverse it.
    return reverse(dst, n, pad(p, buf, width), buf);
}

static int va_printk(char *buf, int n, const char *fmt, va_list args) {
    char *p = buf, *e = buf + n - 1;
    uint32_t x0,x1;

    buf[0] ='\0';
    for(; *fmt && p < e; ) {
        if(*fmt != '%')
            *p++ = *fmt++;
        
        else if(fmt[1] == '%') {
            *p++ = *fmt;
            fmt+=2;
        } else {	
            fmt++;

            char *s, num[128], num1[128];
            unsigned width = 0;

            while(isdigit(*fmt)) {
                width = width*10 + *fmt - '0';
                fmt++;
            }
            // assert(width < 32);

            switch(*fmt) {
            case 'l':
            {
                const char *ty = fmt;
                // assert(strncmp(ty, "llx", 3) == 0 
                //         || strncmp(ty, "lld", 3) == 0);
                fmt += 2;
                uint64_t xx = va_arg(args, uint64_t);
                x0 = xx >> 32;
                x1 = (uint32_t)xx;

                // pull one 32-bit, then pull the other.
                if(strncmp(ty, "llx", 3) == 0) {
                    s = emit(16, num, 128, x0, width, 0);
                    emit(16, num1, 128, x1, width, 0);
                    strcat(num, num1+2);
                } else if(strncmp(ty, "lld", 3) == 0) {
                    s = emit(10, num, 128, x0, width, 0);
                    emit(10, num1, 128, x1, width, 0);
                    // output("num=<%s>, num1=<%s>\n", num, num1);
                    if(strcmp(num,"0") == 0)
                        strcpy(num, num1);
                    else
                        strcat(num, num1);
                } else {
                    // impossible: fmt
                } 
                break;
            }
            
            case 'f':
                // float not enabled!!!
                break;
            case 'd':
                s = emit(10, num, 128, va_arg(args, int), width, 1);
                break;
            case 'u':
                s = emit(10, num, 128, va_arg(args, unsigned), width, 0);
                break;
            case 'p':
            case 'x':
                s = emit(16, num, 128, va_arg(args, unsigned), width, 0);
                break;
            case 'b':
                s = emit(2, num, 128, va_arg(args, unsigned), width, 0);
                break;
            case 's':
                s = va_arg(args, char *);
                break;
            case 'c':
                s = emit(8, num, 128, va_arg(args, int),width,0);
                break;
            default: 
                // error
                buf[0] = '\0';
                return 0;
            }
            fmt++;

            // safe string copy
            for(; p < e && *s; )
                *p++ = *s++;
        }
    }
    *p++ = 0;
    return p - buf;
}

int uart_printk(uart_index uart, const char *fmt, ...) {
    static char buf[512];
    va_list args;

    va_start(args, fmt);
        int sz = va_printk(buf, sizeof buf, fmt, args);
    va_end(args);

    uart_putk(uart, buf);

    return sz;
}
