/* rwhod.c - rwhod */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rwho.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  rwhod  -  Periodically clean cache and (optionally) send rwho packets
 *------------------------------------------------------------------------
 */
PROCESS	rwhod()
{
	STATWORD ps;    
	int	i, j;
	struct	rwent	*rwptr;
	struct	rwent	*myptr;
	struct	rwhopac	*rpacptr;
	struct	rw_who	*rwwptr;
	struct	epacket	*packet;
	IPaddr	mynet;
	long	now;
	int	len;

	/* Initialize rwho information */

	Rwho.rwnent = 1;
	Rwho.rwsend = TRUE;
	getutim(&Rwho.rwbtime);
	myptr = &Rwho.rwcache[0];
	getname(myptr->rwmach);
	myptr->rwboot = myptr->rwlast = myptr->rwslast = Rwho.rwbtime;
	for (i=0 ; i<3 ; i++)
		myptr->rwload[i] = 0L;
	myptr->rwusers = 1;

	getnet(mynet);
	for( ; TRUE ; sleep(RWDELAY) ) {
		getutim(&now);
		myptr->rwlast = myptr->rwslast = now;
		disable(ps);
		for (i=0 ; i<Rwho.rwnent ; i++) {
			rwptr = &Rwho.rwcache[i];
			if (now - rwptr->rwlast > RWMAXDT) {
				Rwho.rwnent--;
				for (j=i-- ; j<Rwho.rwnent ; j++)
				    Rwho.rwcache[j] = Rwho.rwcache[j+1];
			}
		}
		restore(ps);
		if (!Rwho.rwsend)
			continue;
		packet = (struct epacket *)getbuf(Net.netpool);
		rpacptr = (struct rwhopac *)
			((struct udp *)
			(((struct ip *)packet->ep_data)->i_data))->u_data;
		rpacptr->rw_vers = RWVERSION;
		rpacptr->rw_type= RWSTATUS;
		rpacptr->rw_sndtim = hl2net(now);
		rpacptr->rw_rtim = 0L;
		getname(rpacptr->rw_host);
		for (j=0 ; j<RWNLOAD ; j++)
			rpacptr->rw_load[j] = 0L;
		rpacptr->rw_btim = hl2net(Rwho.rwbtime);
		len = RWMINP;
		if ( marked(Shl.shmark) && Shl.shused ) {
			rwwptr = &rpacptr->rw_rww[0];
			strcpy (rwwptr->rw_tty, "Console");
			strncpy(rwwptr->rw_nam, Shl.shuser, RWNLEN);
			rwwptr->rw_ton = hl2net(Shl.shlogon);
			rwwptr->rw_idle = hl2net(now - Shl.shlast);
			len += sizeof(struct rw_who);
		}
		udpsend(mynet, URWHO, URWHO, packet, len);
	}
}
