//
// readsec4.c - display sector contents interactively, featuring 'cf_readsector'  (for 68k-nano target)
//
//
// revisions:
//
// -rolf - may 2, 2021


//#define USE_SIM 1         /* comment this out for non-simulator build */

//#define USE_STDIO_H 1     /* uncomment to compile on modern *nix system */

//#define USE_CTYPE_H 1     /* uncomment to use standard lib ctype.h */


#ifdef USE_STDIO_H
#include <stdio.h>
#endif

#ifdef USE_CTYPE_H
#include <ctype.h>
#endif

#ifndef USE_STDIO_H
#include "libcommon.h"
#endif

// low-level access to CF Card
#include "cfcard.h"


#ifndef USE_STDIO_H

// XXX very temporary! until we work out "FILE *stream" parameters
#define stdin NULL

// XXX need to rename this to strlen()
int knr_strlen(const char s[]);

#endif


typedef unsigned char BYTE;

void print_part(BYTE part[], int pnum);
void print_fatstats(int lba, BYTE data[], int fat16flag);

void dumpbuffer(const BYTE *buf, int size, int do_ascii);


// please choose multiple of 512
#define BUFSIZE 512

// size of buffer for user input
#define STRSIZE 32


// XXX need to implement!
int atoi(const char *str);


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
    int i;
    BYTE buf[BUFSIZE];
    int rc;
    int lba = 0x0;
    char str[STRSIZE];
    char *p;

    
    while (1) {

        minprintf("enter sector: ");
        //fflush(stdout);
        p = fgets(str, STRSIZE, stdin);
        if (p != NULL) {
            chomp(str);
            lba = (int)strtol(str, NULL, 0);  // base 0 means detect base 8, 10 or base 16
        } else {    
            break;
        }

#if 1
        /* initialize with known pattern */
        for (i = 0; i < BUFSIZE; i++) {
            buf[i] = i & 0xff;
        }
    
        minprintf("dump sector %d (0x%x):\n\n", lba, lba);

        rc = cf_readsector(lba, (char *)buf, BUFSIZE);
    
        if (rc != 0)
            minprintf("cf_readsector failed: errcode: %d\n", rc);

        dumpbuffer(buf, BUFSIZE, 1);    // show buffer in hex, with ascii on the right

        minprintf("\n");  // pretty print..
#endif

        minprintf("info for sector %d (0x%x):\n", lba, lba);

        if (lba == 0) {
            // sector 0 is the "master boot record" (MBR)/
            // inside it, we find the partition table and print the entries.
            // that table starts at offset 0x1be, and each entry is 16 bytes.
            // there is usually only 1 partition used, but the table allows for 4.

            BYTE *partp = buf + 0x1BE;
            for (i = 0; i < 4; i++) {
                print_part(partp + 16*i, i);
            }
    
        } else {
            // found boot sector for a partition.. print some stats
            // note: should check for boot sector signature (expect 0x55aa at offset 0x1fe)
            print_fatstats(lba, buf, 1);
        }
    }

    return 0;
}


//
// print interesting fields of a partition table entry
// note: pass along 16 byte array, and also which partition it is (0-3)
//
// special note: the CHS start of partition field at 0x1 offset, is 3 bytes, and is not word-aligned!!
// all word-length numbers in the partition table are represented in little endian
//

void print_part(BYTE part[], int pnum)
{
    int bootable = part[0];  // typically 0x80 (bootable) or 0x0
    
    // not interested in CHS numbers?
    //int start_chs = (part[3] << 16) | (part[2] << 8) | part[1];  // little endian conversion
    
    int part_type = part[4];
    
    // relative offset to the partition in sectors (LBA)
    int start_lba = (part[11] << 24) | (part[10] << 16) | (part[9] << 8) | part[8];
    
    // size of the partition in sectors
    int num_secs = (part[15] << 24) | (part[14] << 16) | (part[13] << 8) | part[12];
    
    // FAT16 should have partition type of 4, 6, or 14.
    // FAT32 should have type 11 or 12.
    // assume that if partition type is 0, then this partition is empty
    if (part_type)
        minprintf("part %d: boot: 0x%x  start_lba: %d (0x%x)  num_secs: %d  type: %d\n", pnum, bootable, start_lba, start_lba, num_secs, part_type);
    else
        minprintf("part %d: <empty>\n", pnum);
}


//
// print some stats about a boot sector.  (only fat16 for now)
//
// the fields within this sector are also called the "Disk Parameter Block".
//
// interpretation of fields depends on whether this is a fat16 or fat32.
// note: fat16flag is currently ignored
//
// credit: compuphase's website has been a great resource for understanding the FAT layout
//  https://www.compuphase.com/mbr_fat.htm
//

void print_fatstats(int lba, BYTE data[], int fat16flag)
{
    // expect 0x55aa at end of sector bytes
    if ((data[0x1fe] != 0x55) || (data[0x1ff] != 0xaa)) {
        minprintf("sector %d (0x%x): not a boot sector!\n");
        return;
    }

    // Sector size in bytes (0x00B - 2 bytes)
    int sec_size =  (data[0x0c] << 8) | data[0x0b];
    
    // Number of sectors per cluster (0x00D - 1 byte)
    int nsecperclus = data[0x0d];
    
    // 0x00E - Reserved sectors (including the boot sector) (2 bytes)
    int rsv_secs =  (data[0x0f] << 8) | data[0x0e];
    
    // 0x010 - Number of FATs (1 byte)
    int numfats = data[0x10];
    
    // 0x011 - Number of directory entries in the root directory (N.A. for FAT32) (2 bytes)
    int numdirs_root =  (data[0x12] << 8) | data[0x11];
    
    // 0x013 - Total number of sectors on the disk/partition - legacy < 32 MB (2 bytes)
    int num_secs1 =  (data[0x14] << 8) | data[0x13];
    
    // 0x016 - Number of sectors used for one FAT table (N.A. for FAT32) (2 bytes)
    int fat_secs =  (data[0x17] << 8) | data[0x16];

    // 0x020 - Total number of sectors on the disk/partition (4 bytes)
    int num_secs2 = (data[0x23] << 24) | (data[0x22] << 16) | (data[0x21] << 8) | data[0x20];
    
    int num_secs = (num_secs1 == 0)? num_secs2 : num_secs1;  // calc which field is used

    // 0x047 - Volume label (this field is at offset 0x02b in FAT12/FAT16)
    // XXX unfinished - needs strncpy or memcpy?

    minprintf("sector %d (0x%x): sec_size: %d  nsecs/clus: %d  nrsv: %d  nfats: %d  fat_secs: %d  ndirs_rt: %d\n", lba, lba, sec_size, nsecperclus, rsv_secs, numfats, fat_secs, numdirs_root);
    minprintf("       nsecs: %d\n", num_secs);
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
            minprintf("%04x  ", (int)(p-buf));
            for (int i = 0; i < 16; i++) {
                if (i == 8)
                    putchar(' ');
                minprintf("%02x ", *p++);
            }
            if (do_ascii) {
                minprintf(" |");
                for (int i = 0; i < 16; i++) {
                    int c = *pp++;
                    if (isprint(c))
                        putchar(c);
                    else
                        putchar('.');  // unprintable char, so print a "."
                }
                minprintf("|");
            }
            minprintf("\n");
        }
    }
}


