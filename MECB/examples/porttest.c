int n;

main()
{
	int prod2(),cons2();
	int produced,consumed,port;

	n = 0;
	port = pcreate(3);
	printf("created port = %d\n",port);
	resume(create(cons2,500,20,"cons",3,consumed,produced,port));
	resume(create(prod2,500,20,"prod",3,consumed,produced,port));
}

char msg[26][6];	/* need separate and stable memory for ea msg */
                        /* --passing ptr thru port here */
prod2(consumed,produced,port)
int consumed,produced,port;
{
	int i;
	char ch='A';	

	printf("port = %d\n",port);
	for (i = 0; i < 26; i++) {
		sprintf(msg[i],"hi %c",ch++);
		psend(port,msg[i]);
		n++;		/* n measures producer progress */
	}
}

cons2(consumed,produced,port)
int consumed,produced,port;
{
	int  i;
	char *msg;

	for (i = 0; i < 26; i++) {
		msg = (char *)preceive(port);
		printf("n=%d, msg = %s\n",n,msg); /* prod, cons progress */
		sleep(1);	/* slow down consumer (not necessary) */
	}
}
