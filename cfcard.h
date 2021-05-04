//
// cfcard.h - low-level access to CF Card
//
//

int cf_readsector(int secnum, char *buf, int bufsize);   // read one sector
