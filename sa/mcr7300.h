/* Miscellaneous control reg, write-only-- */
#define MCR7300 ((unsigned short *)0x4a0000)
/* Dismiss clock int: toggle from high to low and back to high */
#define CLRCLKINT (1<<15)
/* Disk DMA direction: 0 for write, 1 for read */
#define DMARDWRT (1<<14)
/* Data strobe for parallel port: toggle from low to high and back */
#define LPSTR (1<<13)
/* 0=let modem clock feed to 7201 channel B, 1 = use 19.2K clock */
#define MCKSEL (1<<12)
/* Red LED on side of chassis: 0=off, 1=on */
#define LED3 (1<<11)
/* Green LED... */
#define LED2 (1<<10)
/* Yellow LED... */
#define LED1 (1<<9)
/* Red LED... */
#define LED0 (1<<8)
