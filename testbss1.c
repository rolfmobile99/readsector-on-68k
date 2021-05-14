//
// testbss1.c - simple test of C runtime initialization of static storage
//
//
// -rolf - may 12, 2021
//


//#define USE_SIM 1         /* comment this out for non-simulator build */

//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */


#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#include <stdarg.h>  /* needed for minprintf */

#ifndef USE_STDIO_H
/* our application/library interface */
#include "libcommon.h"
#endif


// size of "static int" buffer to test
// 1000 = 0x3e8
#define ASIZE   1000

#define VALUE   0x5a5a5a5a

// note:
// static initialized storage is not necessarily contiguous with uninitialized storage,
// but we check it anyway


static int buf0 = VALUE;
static int zeroes[ASIZE];   // this *should* be automatically zeroed before main() is called
static int buf1 = VALUE;


// let's hope 'ld' exports these for us...
extern int *__bss_start, *_end;


int main(int argc, char *argv[])
{
    int i;
    int failed = 0;

    minprintf("bss test... (%d bytes)\n", (int)sizeof(zeroes));
    
    // additional debug
    minprintf("bss start: 0x%x  bss end: 0x%x\n", &__bss_start, &_end);
    minprintf("array start: 0x%x\n", zeroes);

    for (i = 0; i < ASIZE; i++) {
        if (zeroes[i] != 0) {
            failed = 1;
            break;
        }
    }

    if (failed) {
        minprintf("FAILED: nonzero element found at index %d, value 0x%x\n", i, zeroes[i]);
    } else {
        minprintf("passed.\n");
    }

    if (buf0 != VALUE || buf1 != VALUE) {
        minprintf("FAILED: static initialized data was corrupted! (0x%x or 0x%x != 0x%x)\n", buf0, buf1, VALUE);
    }

    return 0;
}

