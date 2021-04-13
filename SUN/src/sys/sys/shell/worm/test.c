/* test.c -- for playing worm stadalone */
/* simple Xinu worm game by Shawn Ostermann */    

#include <conf.h>
#include <kernel.h>


main()
{
    while (TRUE) {
	playworm();
    }
}
