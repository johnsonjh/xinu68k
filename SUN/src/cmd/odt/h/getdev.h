/* Device database constants */
#define CLASS_SEPER		'%'
#define COMMENT			'#'
#define NEWLINE			'\n'
#define WHITE_SPACE		" \t"
#define FIELD_END		" \t\n"
#define DEV_DB			"/u5/xinu/xinu6/lib/machines"
#define MAXFIELDLEN		60
#define HASHTAB_SIZE		40
#define	DOMAIN_SEP		'.'		/* ip domain separator	*/

/* database macros */
#define get_class(clmap)	(get_field(clmap, 1))  /* db class from map */
#define get_num(clmap)		(get_field(clmap, 2))  /* db num from map   */
#define get_phys(clmap)		(get_field(clmap, 3))  /* phys from cls map */
#define get_limit(clmap)	(get_field(clmap, 4))  /* idle time limit   */
#define	get_args(clmap)		(get_field(clmap, 5))  /* arg string	    */
#define get_name(mach)		(get_field(mach, 1))   /* phys from descrip */
#define get_hname(mach)		(get_field(mach, 2))   /* official host nm  */
#define	get_cputype(mach)	(get_field(mach, 3))   /* cpu type	    */
#define	get_download(mach)	(get_field(mach, 4))   /* downloader binary */
#define	get_upload(mach)	(get_field(mach, 5))   /* uploader binary    */
#define getuser(lock)		(get_field(lock, 1))   /* user from lock fil*/

/* Flag values to getdev */
#define GD_HLDDEVS		(1 << 0)
#define GD_FORCE		(1 << 1)
#define GD_MYDEV		(1 << 2)

/* Request values to getdev */
#define GD_ANYDEV		(-1)

/* getdev constants */
#define MAXNAMELEN		30
#define MAXRECLEN		132
#define SECPERMIN		60
#define FIELDSEP		' '
#define	QUOTE			'"'
#define DEV_ENV			"ODTCLASS"
#define LOCKNAME		"/tmp/xinu/%s"
#define	LOCKDIR			"/tmp/xinu"
#define USRLOCK			"/tmp/xinu/ulock.%s"
#define LOCKMODE		0666
#define ERROR			(-1)
#define INVALID			(-1)
#define MAXTIME			10000000

typedef struct hashent {	/* Device Hash Entry */
	char *machine;			/* ptr to machine description	*/
	struct hashent *next_hash;	/* next entry in hash chain	*/
	} HashEnt;

typedef struct device {		/* Device Descriptor Record */
	int fd;				/* serial line xor lock file fd	*/
	char *locknm;			/* lock file name for dev	*/
	char *machine;			/* ptr to machine description	*/
	char *class;			/* class machine reserved as	*/
	} DevDesc;

char *index(), *skip_white(), *skip_field(), *getlogin(), *getenv();
char *get_db(), *get_field(), *get_locks(), *insrt_dev();
char *lookup_db(), *nextdb_entry(), *find_class(), *find_num();
char *eo_class(), *eo_entry(), *shortname();
DevDesc *getdev(), *onreserve(), *reserve(), *create_lock(), *touch_lock();
DevDesc *getany_dev(), *getthis_dev();
