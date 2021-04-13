/* idprom.h -- structure of the ID prom */

/* Fields stored in the ID prom  */
struct idprom {
	unsigned char	id_format;	/* format identifier */
	unsigned char	id_machine;	/* machine type */
	unsigned char	id_ether[6];	/* ethernet address */
};
