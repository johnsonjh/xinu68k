/* eeprom.h -- structures used for storing info in the Sun3 eeprom */


/* the (hopefully!) unused portion of the Sun3 eeprom space */
#define EEPROMADDR 0x0Fe04710

/* maximum length of the boot file name */
#define MAXFILENAME 40


struct eepromblock {
    IPaddr	myaddr;
    Eaddr	desteaddr;
    IPaddr	destaddr;
    char	fname[MAXFILENAME];
};
