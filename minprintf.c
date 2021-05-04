//
// minprintf.c - library - minimal printf
//
//
// Note: the 'minprintf' code only depends on putchar!
//
//
// revisions:
//
// separate into 4 files:
//  testprintf.c - contains 'main'
//  minprintf.c - library - minimal printf
//  string.c - library - string functions
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


#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#ifdef USE_CTYPE_H
#include <ctype.h>
#endif


#include <stdarg.h>  /* needed for minprintf */

/* our application/library interface */
#include "libcommon.h"


/* strlen:  return length of s */
/*  example code from sec 2.3, p.31 */
/*  note: ++i changed to i++, and add "const char" */

int knr_strlen(const char s[])
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}


#ifdef NOTDEF  /* not used */
/* strlen:  taken from newlib - use for comparison */
int newlib_strlen(const char *str)
{
    const char *start = str;
    while (*str) {
        str++;
    }
    return str - start;
}
#endif


/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = knr_strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


/* itoa:  convert integer n to characters in s */
void knr_itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */

    i = 0;
    do {                /* generate digits in reverse order */
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        s[i++] = '-';

    s[i] = '\0';
    reverse(s);
}


/* unsigned, base 16 (hex) version of above itoa function */
void ux_itoa(unsigned int u, char s[])
{
    int i, rem;
    //const char digits[] = "0123456789ABCDEF";  /* lookup table for hex digits */
    const char digits[] = "0123456789abcdef";  /* lookup table for hex digits */

    i = 0;
    do {                /* generate digits in reverse order */
        rem = u % 16;
        s[i++] = digits[rem];
        u = u / 16;
    } while (u != 0);

    s[i] = '\0';
    reverse(s);
}


/* only signed base 10 and unsigned base 16 are supported */
void my_itoa(int n, char s[], int base)
{
    if (base == 16) {
        ux_itoa((unsigned int)n, s);
    } else {
        knr_itoa(n, s);
    }
}


/* helper function used by 'minprintf' */

char *right_justify_str(const char s[], char t[], int fwidth, int zeroflag)
{
    int n, j, len, rem;
    char pad;

    n = 0;
    len = knr_strlen(s);
    rem = fwidth - len;
    
    if (rem > 0) {
        pad = (zeroflag) ? '0' : ' ';
        while (rem--) {
            t[n++] = pad;
        }
    }
    for (j = 0; j < len; j++) {
        t[n++] = s[j];
    }

    t[n] = '\0';

    return t;
}


#ifdef USE_SIM
#define PUTCHAR_NEWLINE_CHAR 0xa
#endif

/*
    minprintf() - like printf but only allows %s and %d in the format string.
    
    note: originally based on "minprintf()" as described in K&R C Prog Lang, section 7.2
    
    note: on the simulator this also converts the string "\n" to a LF char
*/
void minprintf(char *fmt, ...)
{
	va_list ap;     // pointer to args, if any
	char *p, *sval;
	char c;
	int ival;
	//double dval;
	char sbuf[12];
	char tbuf[12];
#ifdef USE_SIM
	int slashflag = 0;
#endif
	int fmtmode = 0;    // seen '%' and expecting a format
	int zeroflag = 0;   // zero padding specifier
	int fwidth = 0;     // field width specifier


	va_start(ap, fmt);
	for (p = fmt; (c = *p); p++) {

#ifdef USE_SIM
	    if (!slashflag && (c == '\\')) {    // code to handle "\n" in string, on simulator or gcc -S
	        slashflag = 1;
	        continue;
	    }
	    if (slashflag) {
			slashflag = 0;
	        if (c == 'n') {
			    putchar(PUTCHAR_NEWLINE_CHAR);
			    continue;
			} else {
			    putchar('\\');
			    putchar(c);
			    continue;
			}
	    }
#endif

        if (fmtmode) {
            if ((c == '0') && !zeroflag) {
                zeroflag = 1;           // detect leading zero (optional modifier)
			    continue;
            } else if (isdigit(c)) {
                fwidth = c - '0';       // only handles single digit field width
			    continue;
            } else {
                ;  // fall through to 'switch' below
            }
        } else {
            if (c == '%') {
                fmtmode = 1;
                continue;
            } else {
                putchar(c);
                continue;
            }
        }

		switch(c) {                  // handle char that follows '%'
		case 'd':
			ival = va_arg(ap, int);     // extract arg of 'int' type
			sbuf[0] = '\0';             // be safe
			my_itoa(ival, sbuf, 10);
			for (sval = sbuf; *sval; sval++)
				putchar(*sval);
			break;
		case 'x':         /* handles format modifiers e.g. %2x or %04X */
		case 'X':
			ival = va_arg(ap, int);     // extract arg of 'int' type
			sbuf[0] = '\0';             // be safe
			my_itoa(ival, sbuf, 16);
			right_justify_str(sbuf, tbuf, fwidth, zeroflag);  // result in 'tbuf'
			if (c == 'X') {
                for (sval = tbuf; *sval; sval++)
                    putchar(toupper(*sval));
			} else {
                for (sval = tbuf; *sval; sval++)
                    putchar(*sval);
            }
			break;
#if 0
		case 'f':
			dval = va_arg(ap, double);  // extract arg of 'double' type
			printf("%f", dval);
			break;
#endif
		case 's':
			for (sval = va_arg(ap, char *); *sval; sval++)
				putchar(*sval);
			break;
		default:
			putchar(*p);
			break;
		}

		fmtmode = 0;
		zeroflag = 0;
		fwidth = 0;
	}
	va_end(ap);
}


#ifndef USE_STDIO_H
/* taken from MIT xv6 string.c */
/* note: changed 'uint n' to 'int n' */
void*
memmove(void *dst, const void *src, int n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, int n)
{
  return memmove(dst, src, n);
}
#endif
