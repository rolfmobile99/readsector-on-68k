//
// testbss.c - simple test of C runtime initialization of static storage
//
//
// -rolf - may 12, 2021
//


#include <stdio.h>      /* for printf */


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


int main(int argc, char *argv[])
{
    int i;
    int failed = 0;

    printf("bss test... (%d bytes)\n", (int)sizeof(zeroes));

    for (i = 0; i < ASIZE; i++) {
        if (zeroes[i]) {
            failed = 1;
            break;
        }
    }

    if (failed) {
        printf("FAILED: nonzero element found at index %d, value 0x%x\n", i, zeroes[i]);
    } else {
        printf("passed.\n");
    }

    if (buf0 != VALUE || buf1 != VALUE) {
        printf("FAILED: static initialized data was corrupted! (0x%x or 0x%x != 0x%x)\n", buf0, buf1, VALUE);
    }

    return 0;
}

