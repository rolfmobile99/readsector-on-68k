//
// testhex.c - simple test of 'hex dump' feature
//
//
// Note: the 'minprintf' code only depends on putchar!
//
//
// revisions:
//
// -rolf - apr 24, 2021


//#define USE_SIM 1         /* comment this out for non-simulator build */

#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

#define USE_CTYPE_H 1     /* uncomment to use standard lib ctype.h */


#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#ifdef USE_CTYPE_H
#include <ctype.h>
#endif

#ifndef USE_STDIO_H
#include "libcommon.h"
#endif


typedef unsigned char BYTE;

void dumpbuffer(const BYTE *buf, int size, int do_ascii);


// please choose multiple of 256
#define BUFSIZE 512


int main(int argc, char *argv[])
{
    int i;
    BYTE buf[BUFSIZE];
    
    for (i = 0; i < BUFSIZE; i++) {
        buf[i] = i & 0xff;
    }

    dumpbuffer(buf, BUFSIZE, 1); 


    return 0;
}


// note: current limitation: size must be a multiple of 256 !

void dumpbuffer(const BYTE *buf, int size, int do_ascii)
{
    const BYTE *p = buf;
    const BYTE *pp;
    //int i, j, b;

    for (int b = 0; b < size/256; b++) {
        for (int j = 0; j < 16; j++) {
            pp = p;
            printf("%04x  ", (int)(p-buf));
            for (int i = 0; i < 16; i++) {
                if (i == 8)
                    printf(" ");
                printf("%02x ", *p++);
            }
            if (do_ascii) {
                printf(" |");
                for (int i = 0; i < 16; i++) {
                    int c = *pp++;
                    if (isprint(c))
                        putchar(c);
                    else
                        putchar('.');  // unprintable char, so print a "."
                }
                printf("|");
            }
            printf("\n");
        }
    }
}




