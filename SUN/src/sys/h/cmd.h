/* cmd.h */

/* Declarations for all commands known by the shell */

struct	cmdent	{			/* entry in command table	*/
	char	*cmdnam;		/* name of command		*/
	Bool	cbuiltin;		/* Is this a builtin command?	*/
	int	(*cproc)();		/* procedure that implements cmd*/
};

extern	int
	x_bpool(),	x_cat(),	x_close(),	x_conf(),
    	x_cp(),        	x_creat(),	x_date(),	x_devs(),
    	x_dg(),		x_echo(),	x_exit(),	x_help(),
    	x_kill(),	x_mem(),	x_mount(),	x_mv(),
    	x_net(),	x_ps(),		x_reboot(),	x_rf(),
    	x_rls(),	x_rm(),		x_routes(),	x_sleep(),
    	x_unmou(),	x_uptime(),	x_who(),        playworm();

/* Commands:	 name		Builtin?	procedure	*/

#define	CMDS 	"bpool",	FALSE,		x_bpool,	\
		"cat",		FALSE,		x_cat,		\
		"close",	FALSE,		x_close,	\
		"conf",		FALSE,		x_conf,		\
		"cp",		FALSE,		x_cp,		\
		"create",	FALSE,		x_creat,	\
		"date",		FALSE,		x_date,		\
		"devs",		FALSE,		x_devs,		\
		"dg",		FALSE,		x_dg,		\
		"echo",		FALSE,		x_echo,		\
		"exit",		TRUE,		x_exit,		\
		"help",		FALSE,		x_help,		\
		"kill",		TRUE,		x_kill,		\
		"logout",	TRUE,		x_exit,		\
		"mem",		FALSE,		x_mem,		\
		"mount",	FALSE,		x_mount,	\
		"mv",		FALSE,		x_mv,		\
		"netstat",	FALSE,		x_net,		\
		"ps",		FALSE,		x_ps,		\
		"reboot",	TRUE,		x_reboot,	\
		"rf",		FALSE,		x_rf,		\
		"rls",		FALSE,		x_rls,		\
		"rm",		FALSE,		x_rm,		\
		"routes",	FALSE,		x_routes,	\
		"ruptime",	FALSE,		x_uptime,	\
		"sleep",	FALSE,		x_sleep,	\
		"time",		FALSE,		x_date,		\
		"unmount",	FALSE,		x_unmou,	\
		"uptime",	FALSE,		x_uptime,	\
		"who",		FALSE,		x_who,		\
		"worm",		FALSE,		playworm,	\
		"?",		FALSE,		x_help

extern	struct	cmdent	cmds[];
