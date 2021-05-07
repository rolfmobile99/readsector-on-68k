//
// testansi.c - simple test of ANSI escape sequences
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
//  ctype.c - library - ctype functions
//  putchar.c - putchar
//
// -rolf - apr 22, 2021
//


//#define USE_SIM 1         /* comment this out for non-simulator build */

#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

//#define USE_CTYPE_H 1     /* uncomment to use standard lib ctype.h */


#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#ifdef USE_CTYPE_H
#include <ctype.h>
#endif

#include <stdarg.h>  /* needed for minprintf */

#ifndef USE_STDIO_H
/* our application/library interface */
#include "libcommon.h"
#endif


/* test itoa function */
int main(int argc, char *argv[])
{
    printf("hello world\n"); 

    printf("\x1b[32mgreen hello\x1b[0m\n");  // note: no effect on minicom's VT102 emulator

    printf("\x1b[1mbold hello\x1b[0m\n");    // bold text

    printf("\x1b[4munderlined hello\x1b[0m\n");    // underlined text

    printf("  plain hello  \n"); 

    printf("\x1b[7mreversed hello\x1b[0m\n");    // "reversed" text

    printf("goodbye.\n"); 

    return 0;
}

