#include <stdio.h>
#define PACKSIZE  0x400
#define SOH 0x2f
#define ACK 'y'
#define NAK 'n'


loadfile(ldev)
int ldev;
{
  printf("Loading file from dev %d\n",ldev);
  while (getpack(ldev))	/* 0 len returned means eot */
    ;
}

int getpack(ldev)
int ldev;			/* CONSOLE or whatever */
{    
  char Buf[PACKSIZE];	/* temp place for packet */
  int BaseAddr,PackLen;		/* from packet header */
  int CheckSum,PktCkSum;	/* for checksum comparison */
  char ch;
  char *p,*q; 

  while (1) {			/* loop until get a packet */
    do {
      rawputc(ldev,'s');		/* put out 's' until see SOH back */
      printf( " s");
      ch = rawgetc(ldev);
    } while (ch != SOH);
    printf("SOH");
    CheckSum = 0;
    BaseAddr = getlong(ldev,&CheckSum); /* get # and add to cksum */
    PackLen = getlong(ldev,&CheckSum);
    if (PackLen > PACKSIZE) {
      rawputc(ldev,NAK);
      printf("Packlen too long\n");
      continue;			/* getpack loop */
    }
    if (PackLen>0)
      getbuf(ldev,Buf,PackLen,&CheckSum); /* and adjust CheckSum */
    PktCkSum = getlong(ldev);
    if (PktCkSum&CheckSum) {
      rawputc(ldev,ACK); /* lie for now */
 /*   printf("Got BaseAddr %x, Packlen %x\n",BaseAddr,PackLen);
      printf("Checksum err\n"); */
      printf ("x");
      continue;			/* with getpack loop */
    }
      
    for (p=Buf,q=BaseAddr;p<Buf+PackLen;p++,q++)
      *q = *p;    /* copy from Buf to BaseAddr */
    rawputc(ldev,ACK);
    printf("Got BaseAddr %x, Packlen %x\n",BaseAddr,PackLen);
    printf("Sent ACK\n");
    return PackLen;
  }
}

getbuf(ldev,buf,len,cksumptr)
int ldev;
char *buf;
int len;
int *cksumptr;
{
  int i;
  char ch;

  for (i=0;i<len;i++) {
    ch = rawgetc(ldev);
    *buf++ = ch;		/* put char in buf */
    *cksumptr += ch;		/* add to checksum */
  }
}

int getlong(ldev)
int ldev;
{
  int byte0,byte1,byte2,byte3;

  byte0 = rawgetc(ldev); /* low byte first */
  byte1 = rawgetc(ldev);
  byte2 = rawgetc(ldev);
  byte3 = rawgetc(ldev);
  return byte0<<24|byte1<<16|byte2<<8|byte3;
}

int getlong_cs(ldev,cksumptr)
int ldev;
int *cksumptr;
{
  int byte0,byte1,byte2,byte3;

  byte0 = rawgetc(ldev);
  *cksumptr += byte0;
  byte1 = rawgetc(ldev);
  *cksumptr += byte1;
  byte2 = rawgetc(ldev);
  *cksumptr += byte2;
  byte3 = rawgetc(ldev);
  *cksumptr += byte3;
  return byte0<<24|byte1<<16|byte2<<8|byte3;
}
