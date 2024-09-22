#ifndef _ACURSES_H
#define _ACURSES_H

// Agon curses library
// Copyright Shawn Sijnstra (c) 2024
// GPLv3 License
// Please let me know if you use the library, and anything you can add is welcome

// See if we can build a small curses library
// This version is using VDP calls so does not use any terminal characteristics
// A terminal mode version might be nice at a later date
// NOTE: double buffering CAN work on SOME games but only if every frame is a full redraw.

// Note that there is a 512 byte buffer here for expanding out all of the printw style commands.



#include <agon/vdp_vdu.h>
#include <agon/vdp_key.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define CURSES 1
#define CURSES_H 1

/* These are defined only in curses.h, and are used for conditional compiles */
#define NCURSES_VERSION_MAJOR 0
#define NCURSES_VERSION_MINOR 3
#define NCURSES_VERSION_PATCH 0

/* This is defined in more than one ncurses header, for identification */
#undef  NCURSES_VERSION
#define NCURSES_VERSION "0.3"

typedef unsigned chtype;
typedef	chtype	attr_t;		
/* attr_t must be at least as wide as chtype as it is the attributes without the character

chtype has the following bitwise format at the moment:
 upper bit   lower bits
[765][43210] [76543210]
attr  color  character
*/
#define A_CHARTEXT	0x00FF     // Bit-mask to extract character
// I don't think we can do this - A_ATTRIBUTES  Bit-mask to extract attributes

typedef unsigned short WINDOW;

#undef TRUE
#define TRUE    1
#undef FALSE
#define FALSE   0

#undef OK
#define OK	TRUE

/* colors */
#define COLOR_BLACK	0
#define COLOR_RED	1
#define COLOR_GREEN	2
#define COLOR_YELLOW	3
#define COLOR_BLUE	4
#define COLOR_MAGENTA	5
#define COLOR_CYAN	6
#define COLOR_WHITE	7
#define COLOR_BG 128

// not for our warehouse game #define COLORS 16	//otherwise there are too many assumptions about how many colour pairs we can have?
#define COLOR_PAIRS 31
//#define stdscr 0	
#undef stdscr
static WINDOW	*stdscr;  //there is only 1 screen at the moment
#undef curscr
static WINDOW	*curscr;
#define LINES 30	//this really shouldn't be hard coded...
#define COLS 80		//as above. very oldskool approach.

/* CP437
#define ACS_BTEE	0xCA	//'+'	//bottom tee
#define ACS_BULLET	0xFE	//'~' // bullet //
#define ACS_HLINE	0xCD	//'-'	//horizontal line
#define ACS_LANTERN	'#'	//lantern symbol
#define ACS_LLCORNER	0xC8	//'+'	//lower left-hand corner
#define ACS_LRCORNER	0xBC	//'+'	//lower right-hand corner
#define ACS_LTEE	0xCC	//'+'	//left tee
#define ACS_RTEE	0xB9	//'+'	//right tee
#define ACS_TTEE	0xCB	//'+'	//top tee
#define ACS_ULCORNER	0xC9	//'+'	//upper left-hand corner
#define ACS_URCORNER	0xBB	//'+'	//upper right-hand corner
#define ACS_VLINE	0xBA	//'|'	//vertical line
*/

/* CP1252 */
//note: I'm replacing the obelisk with an expanded plus
#define ACS_BTEE	0x86 //'+'	//bottom tee
#define ACS_BULLET	0x95	//'~' // bullet //
#define ACS_HLINE	0x97	//'-'	//horizontal line
#define ACS_LANTERN	'#'	//lantern symbol
#define ACS_LLCORNER	0x86 //'+'	//lower left-hand corner
#define ACS_LRCORNER	0x86 //'+'	//lower right-hand corner
#define ACS_LTEE	0x86 //'+'	//left tee
#define ACS_RTEE	0x86 //'+'	//right tee
#define ACS_TTEE	0x86 //'+'	//top tee
#define ACS_ULCORNER	0x86 //'+'	//upper left-hand corner
#define ACS_URCORNER	0x86 //'+'	//upper right-hand corner
#define ACS_VLINE	'|'	//vertical line
#define ACS_BLOCK	0x81	//block on Agon, normally undefined - by default it's a little small
#define ACS_CKBOARD	0xB1		//this is the CP437 character. We reprogram it later.

#define KEY_UP		11
#define KEY_RIGHT	21
#define KEY_DOWN	10
#define KEY_LEFT	8
#define KEY_ENTER	13
#define KEY_BREAK	27
#define	KEY_MOUSE	255	//placeholder until we work out mouse support
#define KEY_RESIZE	0632		/* Terminal resize event */ //not going to happen on Agon.

#define KEY_F0		0x9f - 1		/* Function keys as they come via vkeycode - F1 is 0x9f */
#define KEY_F(n)	(KEY_F0+(n))	/* Value of function key n */

/*
A_STANDOUT	0	1
A_UNDERLINE	1	2
A_REVERSE	2	4
A_BLINK   	3	8
A_DIM      	4	16
A_BOLD    	5	32
A_INVIS   	6	64
A_PROTECT	7	128
A_ALTCHARSET	8	256
*/
/*
#define A_NORMAL	0
#define A_STANDOUT	1
#define A_UNDERLINE	2
#define A_REVERSE	4
#define A_BLINK		8
#define A_DIM		16
#define A_BOLD		32
*/
// These define the attributes as they are applied in the 16 bit chtype specified above
#define A_NORMAL	0
#define A_STANDOUT	(32 << 8)
#define A_UNDERLINE	0
#define A_REVERSE	0
#define A_BLINK		0
#define A_DIM		0
#define A_BOLD		(32 << 8)

#define getyx(win,y,x)   	(y = getcury(win), x = getcurx(win))
#define getmaxyx(win,y,x)	(y = getmaxy(win), x = getmaxx(win))
#define crmode()		cbreak()
#define nocrmode()		nocbreak()
#define attrset(attrs)	attron(attrs)
#define wattrset(win,attrs)	wattron(win,attrs)
#define fixterm()		reset_prog_mode()
#define resetterm()		reset_shell_mode()
#define saveterm()		def_prog_mode()
#define crmode()		cbreak()
#define nocrmode()		nocbreak()


int addstr(const char *str);
int addch(const chtype ch);
void timeout(int delay);

static short global_color_pairs [COLOR_PAIRS*2];	//global to acurses only

static short _blocking_x = true;							//default is read using _blocking mode


static SYSVAR *sv;
volatile char _curse_buf[512];

volatile KEY_EVENT prev_key_event = { 0 };
volatile char _current_key = 0;

static clock_t _global_delay = 0;

int start_color() {
	// This routine is supposed to define the global variable for number of colours and number of colour pairs
	vdp_set_text_colour( COLOR_WHITE );
	vdp_set_text_colour( COLOR_BLACK + COLOR_BG);
	return true;
};

int erase(void){
    vdp_clear_screen(); return true;
};

int clear(void){
    vdp_clear_screen(); return true;
};

int wclear(WINDOW *win){
    vdp_clear_screen(); return true;
};

int clearok(WINDOW *win, bool state){
    vdp_clear_screen();
    return true;
};

/*
	Normally, the hardware cursor is left at the location of the window cursor being refreshed.
	The leaveok option allows the cursor to be left wherever the update happens to leave it.
	It is useful for applications where the cursor is not used, since it reduces the need for cursor motions.
	Not implemented really here.
*/
int leaveok(WINDOW *win, bool bf){
	return false;
};

void key_event_handler( KEY_EVENT key_event )
{
	if ( key_event.key_data == prev_key_event.key_data ) return;
	else {
	prev_key_event = key_event;
	if (key_event.down != 0) {_current_key = key_event.ascii;}
		else {_current_key = 0;};
	return;
	};
};


WINDOW *initscr() {
	WINDOW *newwin = malloc(sizeof(WINDOW));
	sv = vdp_vdu_init();
//    vdp_mode( 3 );	//Mode 3 is 80x30 with 64 colours
    vdp_mode( 132 ); //is 80x30, 16 colour version with double buffering. Can cause issues.
    vdp_clear_screen();
	vdp_get_scr_dims( true );
    vdp_logical_scr_dims( false );
    vdp_key_init();
    // Replace obelisk with extended "plus"
    vdp_redefine_character( 0x86,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,0x18);
	// technically in CP1252 the vertical line '|' is unbroken, with the 0xA6 being broken.	//vertical line
	vdp_redefine_character( '|',0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
	// create a checkerboard, and put it where it would be for CP 437
	vdp_redefine_character( 0xB1,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA);

    return newwin;
};

int getcury(WINDOW *win)
{
	return sv->cursorY;
};

int getcurx(WINDOW *win)
{
	return sv->cursorX;
};

//#define sysvar_cursorX		    0x07    // 1: Cursor X position
//#define sysvar_cursorY		    0x08    // 1: Cursor Y position
//we might need this:	vdp_get_scr_dims( true );

int getmaxy(WINDOW *win)
{
//	vdp_get_scr_dims( true );
	return sv->scrRows;
};

int getmaxx(WINDOW *win)
{
	return sv->scrCols;
};

// technically we can change colours, but I'm not going to make sure the color provided is valid.
int init_color(short color, short r, short g, short b) {
	vdp_define_colour(color, 255, r, g, b);
	return true;
};

bool has_colors(void) {
	return true;
};

bool can_change_color(void) {
	return true;	// yes the VDU is capable
};

// this should return the content of a programmed colour but I don't think this can be done yet.
int color_content(short color, short *r, short *g, short *b) {
	return false;
};

//The curs_set routine sets the cursor state is set to invisible, normal, or very visible for visibility equal to 0, 1, or 2 respectively. If the terminal supports the visibility requested, the previous cursor state is returned; otherwise, ERR is returned.
// this is a bit of a kludge but should work
int curs_set(int visibility){
	switch (visibility) {
		case 0:
			vdp_cursor_enable( false );
			return 0;
			break;
		case 1:
			vdp_cursor_enable( true );
			return 1;
			break;
		case 2:
			vdp_cursor_enable( true );
			return 2;
			break;
		default:
			return false;
			break;
	};
};

int printw(const char *fmt,...)
{
    va_list argp;
    int code;

    va_start(argp, fmt);
    vsprintf(_curse_buf, fmt, argp);
	code = addstr(_curse_buf);
    va_end(argp);

    return code;
};

int wprintw(WINDOW *win, const char *fmt,...)
{
    va_list argp;
    int code;

    va_start(argp, fmt);
    vsprintf(_curse_buf, fmt, argp);
	code = addstr(_curse_buf);
    va_end(argp);

    return code;
};


//int vw_printw(WINDOW *win, const char *fmt, va_list varglist);

int vw_printw(WINDOW *win, const char *fmt,...)
{
	char *buf;
    va_list argp;
    int code;

    va_start(argp, fmt);
    vsprintf(_curse_buf, fmt, argp);
	code = addstr(_curse_buf);
    va_end(argp);

    return code;
};

int mvprintw(int y, int x, const char *fmt, ...)
{

    va_list argp;
    int code;
    vdp_cursor_tab( x, y);
    va_start(argp, fmt);
    vsprintf(_curse_buf, fmt, argp);
	code = addstr(_curse_buf);
    va_end(argp);

    return code;
};

int mvwprintw(WINDOW *win, int y, int x, const char *fmt, ...)
{

    va_list argp;
    int code;
    vdp_cursor_tab( x, y);
    va_start(argp, fmt);
    vsprintf(_curse_buf, fmt, argp);
	code = addstr(_curse_buf);
    va_end(argp);

    return code;
};

int init_pair(short pair, short f, short b)
{
	if	(pair<COLOR_PAIRS && pair >= 0) {
		global_color_pairs[pair*2] = f;
		global_color_pairs[(pair*2)+1] = b;
		return true;
	} else{
		return false;
	}

};

int pair_content(short pair, short *f, short *b)
{
	if	(pair<COLOR_PAIRS && pair >= 0) {
		*f = global_color_pairs[pair*2];
		*b = global_color_pairs[(pair*2)+1];
		return true;
	} else{
		return false;
	}

};


int COLOR_PAIR (int z)
{
	return (((z) << ((0) + 8)));
};


// VDU 20: Reset palette and text/graphics colours and drawing modes
int use_default_colors(void)
{
	vdp_reset_graphics(); return true;
};

// VDU 23, 0, &C3: Swap the screen buffer and/or wait for VSYNC
// void vdp_swap( void );
// in theory we could use this to draw to the alternate frame, and swap at refresh, thus eliminating flicker.
// double-buffered will mean we have to significantly reduce resolution either in colour palette or screen size.
int refresh(void)
{
	vdp_swap();
	return true;
};

int wrefresh (WINDOW *win)
{
	return refresh();
};


//dump font and reset text mode to bright white on black. NOTE: assumes video mode has at least 16 colours
int endwin(void)
{
	vdp_mode(3);	//single buffer, 64 colours
	vdp_reset_system_font();
	vdp_key_reset_interrupt();
	vdp_set_text_colour( COLOR_WHITE + 8 );
	vdp_set_text_colour( COLOR_BLACK + COLOR_BG);
	vdp_cursor_enable( true );
	return true;
};

//if we always return false, then endwin should be executed
int isendwin(void)
{
	return false;
};

int attron(const chtype attrs)
{
	short color,f,b,bold;
	color = attrs >> 8;
	bold = (color & (A_BOLD >> 8)) >> 2;
	color = (color & COLOR_PAIRS);
	if (color >0)
		{
			pair_content(color,&f,&b);
			vdp_set_text_colour( f + bold);
			vdp_set_text_colour( b + 128);
			}
	else {vdp_set_text_colour(COLOR_WHITE + bold );};
	return true;
};

int wattron(WINDOW *win, int attrs)
{
	return attron(attrs);
};

// I'm currently turning EVERYTHING off, which is not correct, but will do for now.
int attroff(int attrs)
{
	vdp_set_text_colour( COLOR_WHITE );
	vdp_set_text_colour( COLOR_BLACK + COLOR_BG);
	return true;
};

int wattroff(WINDOW *win, int attrs)
{
	return attroff(attrs);
};

// The routine wattr_get returns the current attribute and color pair for the given window
// I'm not currently tracking this. It would need to be done at every attr change.
int attr_get (attr_t *attr, int *n, int *m)
{
	return false;
};

//int wchgat(WINDOW *win, int n, attr_t attr, short color, const void *opts)
//The routine chgat changes the attributes of a given number of characters starting at the current cursor location of stdscr
//It does not update the cursor and does not perform wrapping.
// A character count of -1 or greater than the remaining window width means to change attributes all the way to the end of the current line.
//The wchgat function generalizes this to any window; the mvwchgat function does a cursor move before acting. 
//NOTE: I'm changing the currently applied attributes, so this may not work as expected for any immediately following characters.

int mvwchgat(WINDOW *win, int y, int x, int n, attr_t attr, short color, const void *opts)
{
	int	i;
	uint8_t ch;
    vdp_cursor_tab( x, y); //set up the cursor to make this easier
//uint8_t vdp_return_ascii_code_at_position( int x, int y );
	for (i=x;i<(x+n);i++){
		ch = vdp_return_ascii_code_at_position( i, y);
		addch(ch | attr | (color<<8));
	};

    vdp_cursor_tab( x, y);
    return true;

};

//int wmove(WINDOW *win, int y, int x);
int wmove(WINDOW *win, int y, int x)
{
    vdp_cursor_tab( x, y);

    return true;
};
//void vdp_cursor_tab( int col, int row );

int move(int y, int x)
{
    vdp_cursor_tab( x, y);

    return true;
};

int clrtoeol (void)
{
	return false;
};

int addch(const chtype ch)
{
	short color,f,b,bold;
	color = ch >> 8;
	bold = (color & (A_BOLD >> 8)) >> 2;
	color = (color & COLOR_PAIRS);
	if (color >0)
		{
			pair_content(color,&f,&b);
			vdp_set_text_colour( f + bold); if (b<0) {b=0;}; //assume default is 0. TODO: make this a proper setting
			vdp_set_text_colour( b + 128);
			}
	else {vdp_set_text_colour(COLOR_WHITE + bold );};
	putchar(ch);
	return true;
};

int waddch(WINDOW *win, const chtype ch)
{
	return addch(ch);
}

int mvaddch(int y, int x, const chtype ch)
{
    vdp_cursor_tab( x, y);
    return addch(ch);
};

int mvwaddch(WINDOW *win, int y, int x, const chtype ch)
{
	return mvaddch(y, x, ch);
}

// We don't have echo yet.
int noecho (void)
{
	return true;
};

int echo (void)
{
	return false;
};

int nonl (void)
{
	return false;
};

// The halfdelay routine is used for half-delay mode, which is similar to cbreak mode in that characters typed by the user are immediately available to the program.
//However, after blocking for tenths tenths of seconds, ERR is returned if nothing has been typed. The value of tenths must be a number between 1 and 255. Use nocbreak to leave half-delay mode.
//This is a complete bodge and the timing is wrong.
int halfdelay(int tenths)
{
	timeout(tenths);
	return true;
};

int addstr(const char *str)
{
//	puts(str);
	printf("%s", str);
	return true;
};


int mvaddstr(int y, int x, const char *str)
{
    vdp_cursor_tab( x, y);
    return addstr(str);
};

int mvwaddstr(WINDOW *win, int y, int x, const char *str)
{
	return mvaddstr(y, x, str);
};

int waddstr(WINDOW *win, const char *str)
{
	return addstr(str);
};

int beep(void)
{
	putchar(7);
	return true;
};

//The routine flash flashes the screen, and if that is not possible, sounds the alert. If neither alert is possible, nothing happens. Nearly all terminals have an audible alert (bell or beep), but only some can flash the screen.
// to be sure you're inverting the screen then arguably you should draw a rectangle from -32768,-32768 to 32767,32767 (the highest possible range)
int flash(void)
{
	int	i;
	for (i=0;i<3;i++){
		putchar (25);	//Move cursor
		putchar (0xBC);
		putchar (0);
		putchar (0);
		putchar (0);
		putchar (0);
		putchar (25);	//Invert to bottom corner
		putchar (0x66);
		putchar (1023 & 255);
		putchar (1023 >> 8); //1023
		putchar (1279 & 255);
		putchar (1279 >> 8); //1279
		vdp_swap();
	};
	return true;
};

int keypad(WINDOW *win, bool bf)
{
	return false;
};

//try clearing backlog
int nodelay(WINDOW *win, bool bf)
{
	_blocking_x = !(bf);
	if (bf) {
	vdp_set_key_event_handler( key_event_handler );
	vdp_update_key_state();
	_current_key = 0; //reset any latent keypress
	}
	else {
		vdp_key_reset_interrupt();
	};
	return true;
}

//partial implementation
void wtimeout(WINDOW *win, int delay)
{
	_global_delay = delay;
	if (delay<0)
		{nodelay(win,false);}
	else
		{nodelay(win,true);}
};

void timeout(int delay)
{
	wtimeout(stdscr, delay);
};

char wgetch(WINDOW *win)
{
	clock_t timer = clock() + _global_delay/4;

	char	key;
	if (_blocking_x) {
		key = getch();
		if (key!=0) return key;
		key = sv->vkeycode;
		if (((int)key>KEY_F0)&&((int)key<(KEY_F0+13)))	//maps the function keys to return non-zero values
			{return key;}
		else
			return 0;
	}
	else {
		vdp_update_key_state();key = _current_key;	//need this so that key is collected at least once
		while ( (clock() < timer) && (key = -1)){
		vdp_update_key_state();
		key = _current_key;};
		if (key!=-1) {return key;}
		else
			return 0;
/*
			if (key!=0) return key;
		key = sv->vkeycode;
		if (((int)key>KEY_F0)&&((int)key<(KEY_F0+13)))
			{return key;}
		else
			return 0;
			//test code - doesn't seem to work.
*/
	};
};

//This should save the tty status - there isn't really much to save? Maybe the font colour & attributes?
//Will implement when this comes up so I have something to test.
int savetty()
{
	return true;
};

int resetty()
{
	return true;
};
int raw()
{
	return true;
};
//The cbreak routine disables line buffering and erase/kill character-processing
// (interrupt and flow control characters are unaffected), making characters typed by the user immediately available to the program. 
int cbreak()
{
	return true;
};
// cooked mode - if this works, it should really be waiting for the explicit screen update command.
int nocbreak()
{
	return false;
};

//The timeout and wtimeout routines set blocking or non-blocking read for a given window.
// If delay is negative, blocking read is used (i.e., waits indefinitely for input).
// If delay is zero, then non-blocking read is used (i.e., read returns ERR if no input is waiting).
// If delay is positive, then read blocks for delay milliseconds, and returns ERR if there is still no input.
// Hence, these routines provide the same functionality as nodelay, plus the additional capability of being
// able to block for only delay milliseconds (where delay is positive).

//These are meant to truncate, but for now I will leave as-is.
int hline(chtype ch, int n){
	int	i;
	addch(ch);
	for(i=1;i<n;i++) putchar(ch);
	return true;
};

int whline(WINDOW *win, chtype ch, int n){
	return hline (ch,n);
};

int mvhline(int y, int x, chtype ch, int n){
    vdp_cursor_tab( x, y);
	return hline(ch, n);
};

// vline might cause issues with keyboard input???
int vline(chtype ch, int n){
	int	i; n--;
	addch(ch);
	for(i=1;i<n;i++) {putchar(10);putchar(ch);};
	return true;
};

int wvline(WINDOW *win, chtype ch, int n);

int mvvline(int y, int x, chtype ch, int n){
    vdp_cursor_tab( x, y);
	return vline(ch, n);
};

int mvwvline(WINDOW *, int y, int x, chtype ch, int n);

#define box(win, v, h)		wborder(win, v, v, h, h, 0, 0, 0, 0)
//#define RENDER_WITH_DEFAULT(ch,def) w ## ch = _nc_render(win, (ch == 0) ? def : ch)
#define RENDER_WITH_DEFAULT(ch,def) ch = ((ch == 0) ? def : ch)

int wborder(WINDOW *win,
	chtype ls, chtype rs,
	chtype ts, chtype bs,
	chtype tl, chtype tr,
	chtype bl, chtype br)
{
	int	y,x,curx,cury,i;

	RENDER_WITH_DEFAULT(ls, ACS_VLINE);
    RENDER_WITH_DEFAULT(rs, ACS_VLINE);
    RENDER_WITH_DEFAULT(ts, ACS_HLINE);
    RENDER_WITH_DEFAULT(bs, ACS_HLINE);
    RENDER_WITH_DEFAULT(tl, ACS_ULCORNER);
    RENDER_WITH_DEFAULT(tr, ACS_URCORNER);
    RENDER_WITH_DEFAULT(bl, ACS_LLCORNER);
    RENDER_WITH_DEFAULT(br, ACS_LRCORNER);

    getmaxyx(win,y,x); y--; x--;
    getyx(stdscr, cury, curx);
    // void vdp_cursor_behaviour( int setting, int mask );
    vdp_cursor_behaviour( 0x10, 0xFFEF); // turn off scroll
    mvaddch(0,0,tl);
    mvaddch(y,0,bl);
    mvaddch(0,x,tr);
    mvaddch(y,x,br);
    for (i=1;i<y;i++){
    	mvaddch(i,0,ls);
    	mvaddch(i,x,rs);
    };
/*    for (i=1;i<x;i++){
    	mvaddch(0,i,ts);
    	mvaddch(y,i,bs);
    };
*/
    mvhline(0,1,ts,x);
    mvhline(y,1,bs,x);
    move(cury, curx);
    vdp_cursor_behaviour( 0x10, 0xFFEF); // turn off scroll
	return true;
};

int	napms(int ms)
{
	clock_t timer = clock(); ms=ms/8;
	while ( clock() < timer + ms){};
	return true;
};

chtype mvinch(int y, int x)
{
    // it's unclear if we need to actually move the cursor vdp_cursor_tab( x, y);
    // uint8_t vdp_return_ascii_code_at_position( int x, int y );
    return vdp_return_ascii_code_at_position( x, y );
};

int getnstr(char *str, int n)
{
	fgets(str, n, stdin);
	return true;
};

//we don't really do anything here
int	touchwin(WINDOW *win)
{
	return false;
};

// can we save current terminal settings?
int def_prog_mode(void)
{
	return false;
};

int reset_prog_mode(void)
{
	return false;
};

int reset_shell_mode(void)
{
	return false;
};

#endif //__ACURSES_H
