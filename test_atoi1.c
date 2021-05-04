//
// test_atoi.c - simple test of strtol (superset of atoi) using fgets() for getting input
//
// revisions:
//  may 2, 2021 - initial version.
//
// author:
//  rolf widenfelt
//

//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */


#ifdef USE_STDIO_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* for strtol() */
#else
#include "libcommon.h"
#endif


#ifndef USE_STDIO_H

// XXX very temporary! until we work out "FILE *stream" parameters
#define stdin NULL

// XXX need to rename this to strlen()
int knr_strlen(const char s[]);

#endif


#define STRSIZE 64


//
// chomp - remove a \n char from the end of the string, if there is one.
//
// note: the string str is modified.
//
// inspired by Perl's chomp function.
//
void chomp(char *str)
{
    int len;

    if (str == NULL)
        return;
    len = knr_strlen(str);
    if (len > 0 && (str[len-1] == '\n')) {
        str[len-1] = '\0';
    }
}


int main(int argc, char *argv[])
{
    int done = 0;
    char str[STRSIZE];
    char *p;
    int val;
    
    //
    // loop asking for input (with fgets) and converts the string to an integer.
    //      the string can be base 8 (e.g. 0777), 10 (-1234) or 16 (0xff).
    //      leading space is ignored.
    //
    // fgets() is used instead of gets() because the latter "allows" buffer overruns.
    //  for that reason gets() is marked as "obsolete" in POSIX and is removed under "C11".
    //
    // note: standard fgets() will include in the string the \n typed by the user!
    //  (however, ctrl-D will only return NULL)
    //  it might be nice to add code to remove the \n, if it appears, and then replace it with a \0.
    //      yes! added chomp().
    //
    
    //printf("sizeof(size_t): %d\n\n", (int)sizeof(size_t));  // DEBUG - strlen returns a size_t

    while (!done) {

        minprintf("enter digits: ");
        //fflush(stdout);
        p = fgets(str, STRSIZE, stdin);
        
        if (p != NULL) {
            chomp(str);
            val = (int)strtol(str, NULL, 0);             // base 0 means detect base 8, 10 or base 16
            minprintf("strtol: %d (0x%x)\n", val, val);
        } else {    
            done = 1;
        }
    }
    minprintf("\n");

    return 0;
}
