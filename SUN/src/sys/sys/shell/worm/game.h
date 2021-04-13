/*
 * This file gives the definitions for printing to a particular terminal.
 * If it doesn't work on your particular terminal, then you will need to
 * to change it yourself.  Try doing a man on "termcap" and a
 * "printenv TERMCAP" to see what terminal type you are using.
 *
 */

/* cursor addressable string for printing to terminal */
#define CM		"\033[%d;%dH"

/* command to clear the screen */
#define CLEARSCREEN "\033[H\033[2J"    

/* size of the available window */
#define XMAX 75
#define YMAX 22

/* size of the actual playing field */
#define XMAXFIELD XMAX
#define YMAXFIELD (YMAX-2)
    

/* definitions of the user keys */
#define NORTH                   'k'
#define NE                              'u'
#define NW                'y'    
#define EAST                                  'l'
#define WEST       'h'         
#define SE                               'n'
#define SW                'b'    
#define SOUTH                   'j'
#define SIT     ' '
#define QUIT 'Q'

/* pid's of the various processes */
extern int pidmover;
extern int pidplotter;
extern int pidtargeter;
extern int pidreferee;
extern int pidmain;
extern int pidrobot;

/* current location of the target */
extern int targetx;
extern int targety;
extern int targetvalue;

/* current location of the robot */
extern int robotx;
extern int roboty;

/* current location of the user's worm */
extern int wormx;
extern int wormy;


