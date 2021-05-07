//
// testansi1.c - simple test of ANSI escape sequences using 'minprintf' library
//
// Resources:
//  Li Haoyi's excellent post on ANSI codes:
//    https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
//
//
//
// -rolf - may 4, 2021
//

/***
  interesting comment on Li Haoyi's post:

    Actually... "[1m" means "start bold text", and doesn't actually say anything about 
    brightness or color for neither background nor foreground. 
    The thing that gets people confused is that many (if not most) terminals actually 
    display bold text as bright OR as bold+bright....

    0=reset
    1=bold
    4=underline
    30-37=normal fg colors
    40-47=normal bg colors
    90-97=bright fg colors
***/


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
    minprintf("hello world\n"); 

    minprintf("\x1b[32mgreen hello\x1b[0m\n");  // note: no effect on minicom's VT102 emulator

    minprintf("\x1b[1mbold hello\x1b[0m\n");    // bold text

    minprintf("\x1b[4munderlined hello\x1b[0m\n");    // underlined text

    minprintf("  plain hello  \n"); 

    minprintf("\x1b[7mreversed hello\x1b[0m\n");    // "reversed" text

    minprintf("goodbye.\n"); 

    return 0;
}

