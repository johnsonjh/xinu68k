/* x_conf.c - x_conf */

#include <conf.h>
#include <kernel.h>
#include <network.h>


/*------------------------------------------------------------------------
 *  x_conf  -  Print info about the system configuration
 *------------------------------------------------------------------------
 */
COMMAND x_conf(stdin, stdout, stderr, nargs, args)
int     stdin, stdout, stderr, nargs;
char    *args[];
{
        char    str[80];
        char    name[80];
        char *FindName();

        sprintf(str,"Identification\n");
        write(stdout, str, strlen(str));

        sprintf(str,"    Xinu version:     %s\n",VERSION);
        write(stdout, str, strlen(str));

        sprintf(str,"    Ethernet address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                eth[0].etpaddr[0]&0xff,
                eth[0].etpaddr[1]&0xff,
                eth[0].etpaddr[2]&0xff,
                eth[0].etpaddr[3]&0xff,
                eth[0].etpaddr[4]&0xff,
                eth[0].etpaddr[5]&0xff);
        write(stdout, str, strlen(str));
        
        sprintf(str,"    IP address:       %d.%d.%d.%d\n",
                (unsigned short) Net.myaddr[0]&0xff,
                (unsigned short) Net.myaddr[1]&0xff,
                (unsigned short) Net.myaddr[2]&0xff,
                (unsigned short) Net.myaddr[3]&0xff);
        write(stdout, str, strlen(str));

        sprintf(str,"    Domain name:      %s\n",
                ip2name(Net.myaddr,name) == SYSERR ? "UNKNOWN" : name);
        write(stdout, str, strlen(str));

        sprintf(str,"\nTable sizes\n");
        write(stdout, str, strlen(str));

        sprintf(str,"    Number of processes:   %d\n",NPROC);
        write(stdout, str, strlen(str));

        sprintf(str,"    Number of semaphores:  %d\n",NSEM);
        write(stdout, str, strlen(str));

        sprintf(str,"    Number of devices:     %d\n",NDEVS);
        write(stdout, str, strlen(str));

        sprintf(str,"\nNetwork information\n");
        write(stdout, str, strlen(str));

#ifdef GATEWAY
        sprintf(name,"%d.%d.%d.%d",GATEWAY);
        sprintf(str,"    Network gateway:    %d.%d.%d.%d            (%s) \n",
                GATEWAY, FindName(name));
        write(stdout, str, strlen(str));
#endif  

#ifdef TSERVER
        sprintf(str,"    Time server:        %-21s (%s)\n",
                TSERVER, FindName(TSERVER));
        write(stdout, str, strlen(str));
#endif  

#ifdef RSERVER
        sprintf(str,"    Remote file server: %-21s (%s)\n",
                RSERVER, FindName(RSERVER));
        write(stdout, str, strlen(str));
#endif  

#ifdef NSERVER
        sprintf(str,"    Domain name server: %-21s (%s)\n",
                NSERVER, FindName(NSERVER));
        write(stdout, str, strlen(str));
#endif  

        return(OK);
}

char *FindName(strIP)
     char *strIP;
{
    static char name[80];
    IPaddr addr;
    int tmp[4];
    sscanf(strIP,"%d.%d.%d.%d",&tmp[0], &tmp[1], &tmp[2], &tmp[3]);
    dot2ip(addr,tmp[0], tmp[1], tmp[2], tmp[3]);
    
    if (ip2name(addr,name) == SYSERR)
        return("UNKNOWN");
    else
        return(name);
}
