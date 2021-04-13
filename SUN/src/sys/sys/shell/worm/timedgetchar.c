#include <kernel.h>
#include <conf.h>
#include <io.h>

int sleeper(), reader();
extern int pids, pidr;

#define WAKEUP -1

timedgetchar(howlong)
     int howlong;
{
    STATWORD ps;
    int ret;

    recvclr();
    resume(pids = create(sleeper,200,20,"sleeper",2,getpid(),howlong));
    resume(pidr = create(reader,200,20,"reader",1,getpid()));
    disable(ps);
    ret = receive();
    kill(pidr);
    kill(pids);
    pidr = pids = 0;
    restore(ps);
    return(ret);
}


sleeper(pid,count)
     int pid;
     int count;
{
    sleep10(count);
    send(pid,WAKEUP);
}


reader(pid)
     int pid;
{
    send(pid,getchar());
}
