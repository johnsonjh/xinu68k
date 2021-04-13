/* doprnt.c - _doprnt, _prt10, _prtl10, _prt8, _prtl8, _prt16, _prtl16 */

#define	MAXSTR	80
#define	LOCAL	static
#define disable()	asm("movw sr, sp@-"); asm("movw #0x2700, sr");
#define restore()	asm("movw sp@+, sr");

/*------------------------------------------------------------------------
 *  _doprnt --  format and write output using 'func' to write characters
 *------------------------------------------------------------------------
 *
 *    patched for Sun3 by Shawn Ostermann
 *            all arguments passed as 4 bytes, long==int
 *
 */
_doprnt(fmt, args, func, farg)	/* adapted by S. Salisbury, Purdue U.	*/
	char	*fmt;		/* Format string for printf		*/
	int	*args;		/* Arguments to	printf			*/
	int	(*func)();	/* Function to put a character		*/
	int	farg;		/* Argument to func			*/
{
	int	c;
	int	i;
	int	f;		/* The format character	(comes after %)	*/
	char	*str;		/* Running pointer in string		*/
	char	string[20];	/* The string str points to this output	*/
				/*  from number conversion		*/
	int	length;		/* Length of string "str"		*/
	char	fill;		/* Fill	character (' ' or '0')		*/
	int	leftjust;	/* 0 = right-justified,	else left-just.	*/
	int	longflag;	/* != 0	for long numerics - not	used	*/
	int	fmax,fmin;	/* Field specifications	% MIN .	MAX s	*/
	int	leading;	/* No. of leading/trailing fill	chars.	*/
	char	sign;		/* Set to '-' for negative decimals	*/
	char	digit1;		/* Offset to add to first numeric digit	*/

	for(;;) {
		/* Echo characters until '%' or end of fmt string */
		while( (c = *fmt++) != '%' ) {
			if( c == '\0' )
				return;
			(*func)(farg,c);
		}
		/* Echo "...%%..." as '%' */
		if( *fmt == '%' ) {
			(*func)(farg,*fmt++);
			continue;
		}
		/* Check for "%-..." == Left-justified output */
		if (leftjust = ((*fmt=='-') ? 1 : 0) )
			fmt++;
		/* Allow for zero-filled numeric outputs ("%0...") */
		fill = (*fmt=='0') ? *fmt++ : ' ';
		/* Allow for minimum field width specifier for %d,u,x,o,c,s*/
		/* Also allow %* for variable width (%0* as well)	*/
		fmin = 0;
		if( *fmt == '*' ) {
			fmin = *args++;
			++fmt;
		}
		else while( '0' <= *fmt && *fmt <= '9' ) {
			fmin = fmin * 10 + *fmt++ - '0';
		}
		/* Allow for maximum string width for %s */
		fmax = 0;
		if( *fmt == '.' ) {
			if( *(++fmt) == '*' ) {
				fmax = *args++;
				++fmt;
			}
		else while( '0' <= *fmt && *fmt <= '9' ) {
			fmax = fmax * 10 + *fmt++ - '0';
			}
		}
		/* Check for the 'l' option to force long numeric */
		if( longflag = ((*fmt == 'l') ? 1 : 0) )
			fmt++;
#ifndef INT_NOT_LONG
		longflag = 1;	/* all are 4 bytes on the Sun */
#endif				
		
		str = string;
		if( (f= *fmt++) == '\0' ) {
			(*func)(farg,'%');
			return;
		}
		sign = '\0';	/* sign == '-' for negative decimal */

		switch( f ) {
		    case 'c' :
			string[0] = (char) *args;
			string[1] = '\0';
			fmax = 0;
			fill = ' ';
			break;

		    case 's' :
			str = (char *) *args;
			fill = ' ';
			break;

		    case 'D' :
			longflag = 1;
		    case 'd' :
			if (longflag) {
				if ( *(long *)args < 0) {
					sign = '-';
					*(long *)args = -*(long *)args;
				}
			} else {
				if ( *args < 0 ) {
					sign = '-';
					*args = -*args;
				}
			}
			longflag--;
		    case 'U':
			longflag++;
		    case 'u':
			if( longflag ) {
				digit1 = '\0';
				/* "negative" longs in unsigned format	*/
				/* can't be computed with long division	*/
				/* convert *args to "positive", digit1	*/
				/* = how much to add back afterwards	*/
				while(*(long *)args < 0) {
					*(long *)args -= 1000000000L;
					++digit1;
				}
				_prtl10(*(long *)args, str);
				str[0] += digit1;
#ifdef INT_NOT_LONG				
				++args;
#endif				
			} else
				_prt10(*args, str);
			fmax = 0;
			break;

		    case 'O' :
			longflag++;
		    case 'o' :
			if ( longflag ) {
				_prtl8(*(long *)args, str);
#ifdef INT_NOT_LONG				
				++args;
#endif				
			} else
				_prt8(*args, str);
			fmax = 0;
			break;

		    case 'X' :
			longflag++;
		    case 'x' :
			if( longflag ) {
				_prtl16(*(long *)args, str);
#ifdef INT_NOT_LONG				
				++args;
#endif				
			} else
				_prt16(*args, str);
			fmax = 0;
			break;

		    case 'B' :
			longflag++;
		    case 'b' :
			if( longflag ) {
				_prtl2(*(long *)args, str);
#ifdef INT_NOT_LONG				
				++args;
#endif				
			} else
				_prt2(*args, str);
			fmax = 0;
			break;

		    default :
			(*func)(farg,f);
			break;
		}
		args++;
		for(length = 0; str[length] != '\0'; length++)
			;
		if ( fmin > MAXSTR || fmin < 0 )
			fmin = 0;
		if ( fmax > MAXSTR || fmax < 0 )
			fmax = 0;
		leading = 0;
		if ( fmax != 0 || fmin != 0 ) {
			if ( fmax != 0 )
				if ( length > fmax )
					length = fmax;
			if ( fmin != 0 )
				leading = fmin - length;
			if ( sign == '-' )
				--leading;
		}
		if( sign == '-' && fill == '0' )
			(*func)(farg,sign);
		if( leftjust == 0 )
			for( i = 0; i < leading; i++ )
				(*func)(farg,fill);
		if( sign == '-' && fill == ' ' )
			(*func)(farg,sign);
		for( i = 0 ; i < length ; i++ )
			(*func)(farg,str[i]);
		if ( leftjust != 0 )
			for( i = 0; i < leading; i++ )
				(*func)(farg,fill);
	}

}

LOCAL	_prt10(num,str)
	unsigned int num;
	char	*str;
{
	int	i;
	char	c, temp[6];

	temp[0]	= '\0';
	for(i = 1; i <= 5; i++)  {
		temp[i] = num % 10 + '0';
		num /= 10;
	}
	for(i =	5; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prtl10(num,str)
	long	num;
	char	*str;
{
	int	i;
	char	c, temp[11];

	temp[0]	= '\0';
	for(i = 1; i <= 10; i++)  {
		temp[i] = num % 10 + '0';
		num /= 10;
	}
	for(i = 10; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prt8(num,str)
	unsigned int num;
	char	*str;
{
	int	i;
	char	c;
	char	temp[7];

	temp[0]	= '\0';
	for(i = 1; i <= 6; i++)  {
		temp[i]	= (num & 07) + '0';
		num = (num >> 3) & 0037777;
	}
	temp[6] &= '1';
	for(i = 6; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prtl8(num,str)
	long	num;
	char	*str;
{
	int	i;
	char	c, temp[12];

	temp[0]	= '\0';
	for(i = 1; i <= 11; i++)  {
		temp[i]	= (num & 07) + '0';
		num = num >> 3;
	}
	temp[11] &= '3';
	for(i = 11; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prt16(num,str)
	unsigned int num;
	char	*str;
{
	int	i;
	char	c, temp[5];

	temp[0] = '\0';
	for(i = 1; i <= 4; i++)  {
		temp[i] = "0123456789abcdef"[num & 0x0f];
		num = num >> 4;
	}
	for(i = 4; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prtl16(num,str)
	long	num;
	char	*str;
{
	int	i;
	char	c, temp[9];

	temp[0] = '\0';
	for(i = 1; i <= 8; i++)  {
		temp[i] = "0123456789abcdef"[num & 0x0f];
		num = num >> 4;
	}
	for(i = 8; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}



LOCAL	_prt2(num,str)
	unsigned int num;
	char	*str;
{
	int	i;
	char	c, temp[17];

	temp[0] = '\0';
	for(i = 1; i <= 16; i++)  {
		temp[i] = ((num%2) == 0) ? '0' : '1';
		num = num >> 1;
	}
	for(i = 16; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}



LOCAL	_prtl2(num,str)
	long num;
	char	*str;
{
	int	i;
	char	c, temp[35];

	temp[0] = '\0';
	for(i = 1; i <= 32; i++)  {
		temp[i] = ((num%2) == 0) ? '0' : '1';
		num = num >> 1;
	}
	for(i = 32; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}


