/*
**
**    Description:
**      .ld file for m68k-elf-ld for 68k-nano board
**
**
**    History:
**      14 may 2021 - rolf
**          ported to 68k-nano board.
**
**      12th Apr 2021   roderick	Created.
*/


SEARCH_DIR(.)

/*
** This makes sure crt0.o is the first file to be linked.
**
** DO NOT INCLUDE crt.o on the LD line in the Makefile
*/
STARTUP(crt0.o)


OUTPUT_ARCH(m68k)


PROVIDE (__stack = 0x00efc000);


MEMORY 
{
    rom (rx) :  ORIGIN = 0x00000000, LENGTH = 64K
    ram (rwx):  ORIGIN = 0x00E00000, LENGTH = 1M
}


SECTIONS
{
    /*
    ** Code and Constants
    */

    .text 0xE00100 :    /* actual start of user program in RAM (first 0x100 bytes reserved) */
    {
	__s_text = .;

	crt0.o(.text)

	. = ALIGN(0x4);
	*(.text)
	*(.code)
	*(CODE)

	__e_text = .;
    } > ram


    /*
    ** Initialised data
    **
    **  global/static variables which must have defined values on startup
    */
    .data : 
    {
	. = ALIGN(0x4);
	__s_data = .;

	*(.rodata)

	. = ALIGN(0x4);
	*(.data)

    *(.comment)     /* we really don't need this! */

	__e_data = .;
    } > ram


    /*
    ** Uninitialised data
    */
    .bss :
    {
	. = ALIGN(0x4);
	__bss_start = .;

	*(.bss)

	__bss_end = .;
	_end = .;

	. = ALIGN(0x4);
    __heap_start = .;

    } > ram
} 
