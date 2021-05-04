//
// testprintf.c - simple test of 'minprintf' library
//
//
// Note: the 'minprintf' code only depends on putchar!
//
//
// revisions:
//
// separate into 5 files:
//  testprintf.c - contains 'main'
//  minprintf.c - library - minimal printf
//  string.c - library - string functions
//  ctype.c - library - ctype functions
//  putchar.c - putchar
//
// -rolf - apr 22, 2021
//
// add support for format modifiers, like leading zero and field width (for %x only!).
// for example, %2x and %08x are supported. also if you use %X, the hex digits are in caps.
// -rolf - apr 21, 2021
//
// start with itoa3, which has basic minprintf that supports %d and %x
// -rolf - apr 19, 2021
//
// take main code from itoa2, and add in minprintf that supports %d and %x
// -rolf - apr 4, 2021
//
// copied from my_itoa.c (based on itoa.c from K&R C manual - 2nd edition)
// -rolf mar 2021
//


//#define USE_SIM 1         /* comment this out for non-simulator build */

//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

//#define USE_CTYPE_H 1     /* uncomment to use standard lib ctype.h */


#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#ifdef USE_CTYPE_H
#include <ctype.h>
#endif

#include <stdarg.h>  /* needed for minprintf */

/* our application/library interface */
#include "libcommon.h"


/* test itoa function */
int main(int argc, char *argv[])
{
    const int tests[6] = {3, 99, 256, -100, 65536, 2147483647};  /* last value is 2^31-1 */
    int i;
    
    for (i = 0; i < sizeof(tests)/sizeof(int); i++) {
        minprintf("%d (0x%x)\n", tests[i], tests[i]);         // behaves like a minimal printf()
    }

    minprintf("%4X\n", 0x5); 
    minprintf("%4X\n", 0xf5); 
    minprintf("%04X\n", 0x5); 
    minprintf("%04X\n", 0xf5); 

    minprintf("0x%08x\n", 0xf5f6); 
    minprintf("0x%08x\n", 0xf5f6f7f8); 

    // now test %02x
    unsigned char bytes[] = {0x05, 0xF0, 0x0F, 0x78, 0x0};
    minprintf("bytes: %02x %02x %02x %02x %02x\n", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4]); 


    return 0;
}

