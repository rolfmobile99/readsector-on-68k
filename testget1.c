//
// testget1.c - simple interactive test of fgets() function (on 68k nano)
//
// revisions:
//
// -rolf - may 2, 2021


//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */


#ifdef USE_STDIO_H
#include <stdio.h>
#include <string.h>
#else
#include "libcommon.h"
#endif


#define STRSIZE 64


#ifndef USE_STDIO_H

// XXX very temporary! until we work out "FILE *stream" parameters
#define stdin NULL

// XXX need to rename this to strlen()
int knr_strlen(const char s[]);

#endif


//
// removes a \n char from the end of the string, if there is one.
//
// note: the string str is modified.
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
    
    //
    // loop asking for input (with fgets) and then echoing it back (with printf).
    //
    // fgets() is used instead of gets() because the latter "allows" buffer overruns.
    //  for that reason gets() is marked as "obsolete" in POSIX and is removed under "C11".
    //
    // note: fgets() will include in the string the \n typed by the user!
    //  (however, ctrl-D will only return NULL)
    //  it might be nice to add code to remove the \n, if it appears, and then replace it with a \0.
    //      yes! added chomp().
    //      note that chomp() is only needed below *if* fgets is fixed to return \n.
    //
    while (!done) {

        minprintf("enter: ");
        //fflush(stdout);
        p = fgets(str, STRSIZE, stdin);
        
        if (p != NULL) {
            chomp(str);
            minprintf("you typed %d chars: %s\n", (int)knr_strlen(str), str);
        } else {    
            done = 1;
        }
    }
    minprintf("\n");

    return 0;
}
