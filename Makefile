#
# Makefile - template for "68k-nano" projects
#
# hardware target: 68k-nano
#
#
# REVISIONS:
# -rolf  apr 25, 2021
#       merged rim's Makefile for the CB030, with a couple others.
#       lots of futzing to get a binary that worked, esp with the 'ld' step.
#       replaced gnu as with vasm, to allow a "more readable" asm language.
#
# -roderick     apr 16, 2021
#       modified Joao Silva's Makefile to create a linked binary that can be run on the 
#       Shen.
#
# NOTES:
#  -fomit-frame-pointer
#      Omit frame pointer creation in functions that don’t need it --- functions that have
#      no local variables, for example.
#

CC	= m68k-elf-gcc

# use vasm instead of gcc assembler
#AS	= m68k-elf-as
AS=vasmm68k_mot

LD	= m68k-elf-ld

#AR	= m68k-elf-ar

OBJDUMP	= m68k-elf-objdump
OBJCOPY	= m68k-elf-objcopy

TARGET	= m68000
LDMACH	= 68k-nano.ld 

CFLAGS =-$(TARGET) -Wall -msoft-float -fomit-frame-pointer -Wuninitialized

ASFLAGS=-Felf -$(TARGET)

#LDFLAGS = -T$(LDMACH) -z norelro -v -nostartfiles -nolibc -nodefaultlibs -nostdlib -Map test.map
LDFLAGS	= -z norelro -v -nostartfiles -nolibc -nodefaultlibs -nostdlib
OCFLAGS = -v -p -O binary 


# put the filename of your one page app in APP (and a "8.3" limited name in APP_FATNAME)

APP=readsec4
APP_FATNAME=readsec4.bin

LIBS=cfcard.o minprintf.o strtol.o ctype.o putchar.o getchar.o 

# just documenting which asm files are used
ASMSRCS=crt0.asm cfcard.asm putchar.asm getchar.asm

all: $(APP)

#
# Rules
#

$(APP): $(LIBS)

#
# compile a C file. we use gcc to generate the .o directly.
#
%.o: %.c
	$(CC) $(CFLAGS) -c $<

#
# assemble a standalone "asm" file - we use vasm, not gcc.
# note: the output file is ".o", which can be linked later.
#
%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<


#crt0.o: crt0.asm
#	$(AS) $(ASFLAGS) -o $@ $<

#
# Do not include crt0.o as the .ld file takes care of that.
# XXX for now, we include crt0.o (it must be first!!)
#

$(APP): crt0.o $(APP).o $(LIBS)
	$(LD) $(LDFLAGS) -Ttext=0xe00100 -Map $(APP).map -o $(APP).elf crt0.o $(APP).o $(LIBS)
	$(OBJCOPY) $(OCFLAGS) $(APP).elf $(APP).bin
	#/bin/cp $(APP).bin $(APP_FATNAME)


clean:
	rm *.o
	rm $(APP).map

