//
// ctype.c - library - ctype functions subset
//
//


//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

//#define USE_CTYPE_H 1     /* uncomment to use standard lib ctype.h */


/* our application/library interface */
#include "libcommon.h"


#ifndef USE_CTYPE_H

int isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int toupper(int c)
{
    return (c >= 'a' && c <= 'z') ? (c - 0x20) : c ;
}

int tolower(int c)
{
    return (c >= 'A' && c <= 'Z') ? (c + 0x20) : c ;
}

int isprint(int c)
{
    return (c >= 0x20 && c <= 0x7e);  // range of ascii printable chars
}

int isspace(int c)
{
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');  // various whitespace chars (skipped \v \f)
}

#endif
