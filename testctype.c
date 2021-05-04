//
// testctype.c - quick check of isdigit() and a few other "ctype.h" functions
//
// note: in many implementations, these functions are actually macros.
//


#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

//#define USE_CTYPE_H 1     /* uncomment to use standard lib ctype.h */

#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#ifdef USE_CTYPE_H
#include <ctype.h>

#else
int isdigit(int c);
int isalpha(int c);
int tolower(int c);
int toupper(int c);
#endif



/* test ctype functions */
int main(int argc, char *argv[])
{
    const char tests[10] = {'c', '2', '%', 'X', '9', '@', '^', 'A', 'z', '!'};
    int i;
    
    printf("test isdigit...\n");
    for (i = 0; i < sizeof(tests)/sizeof(char); i++) {
        printf("char: %c isdigit: %d  isalpha: %d\n", tests[i], isdigit(tests[i]), isalpha(tests[i]));
    }

    printf("\n");

    printf("test tolower...\n");
    for (i = 0; i < sizeof(tests)/sizeof(char); i++) {
        printf("char: %c tolower: %c  toupper: %c\n", tests[i], tolower(tests[i]), toupper(tests[i]));
    }

    return 0;
}




#ifndef USE_CTYPE_H

int isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int isalpha(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int tolower(int c)
{
    return (c >= 'A' && c <= 'Z') ? (c + 0x20) : c ;
}

int toupper(int c)
{
    return (c >= 'a' && c <= 'z') ? (c - 0x20) : c ;
}

#endif
