// Agon curses library
// Copyright Shawn Sijnstra (c) 2024
// GPLv3 License
// Please let me know if you use the library, and anything you can add is welcome

// See if we can build a small curses library
// This version is using VDP calls so does not use any terminal characteristics
// A terminal mode version might be nice at a later date

// Note that there is a 512 byte buffer here for expanding out all of the printw style commands.

#include <agon/vdp_vdu.h>
#include <stdio.h>

#ifndef __NCURSES_H
#define __NCURSES_H

#define CURSES 1
#define CURSES_H 1

/* These are defined only in curses.h, and are used for conditional compiles */
#define NCURSES_VERSION_MAJOR 0
#define NCURSES_VERSION_MINOR 1
#define NCURSES_VERSION_PATCH 0

/* This is defined in more than one ncurses header, for identification */
#undef  NCURSES_VERSION
#define NCURSES_VERSION "0.1"

#endif

typedef unsigned chtype;
typedef	chtype	attr_t;		/* ...must be at least as wide as chtype */

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

#define COLORS 16	//otherwise there are too many assumptions about how many colour pairs we can have?
#define COLOR_PAIRS 31
#define stdscr 0	//there is only 1 screen

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
#define ACS_BTEE	'+'	//bottom tee
#define ACS_BULLET	0x95	//'~' // bullet //
#define ACS_HLINE	0x97	//'-'	//horizontal line
#define ACS_LANTERN	'#'	//lantern symbol
#define ACS_LLCORNER	'+'	//lower left-hand corner
#define ACS_LRCORNER	'+'	//lower right-hand corner
#define ACS_LTEE	'+'	//left tee
#define ACS_RTEE	'+'	//right tee
#define ACS_TTEE	'+'	//top tee
#define ACS_ULCORNER	'+'	//upper left-hand corner
#define ACS_URCORNER	'+'	//upper right-hand corner
#define ACS_VLINE	'|'	//vertical line

#define KEY_UP	11
#define KEY_RIGHT 21
#define KEY_DOWN 10
#define KEY_LEFT 8
#define KEY_ENTER	13
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
#define A_NORMAL	0
#define A_STANDOUT	0
#define A_UNDERLINE	0
#define A_REVERSE	0
#define A_BLINK		0
#define A_DIM		0
#define A_BOLD		(32 << 8)

#define getyx(win,y,x)   	(y = getcury(win), x = getcurx(win))
#define getmaxyx(win,y,x)	(y = getmaxy(win), x = getmaxx(win))
int addstr(const char *str);

short global_color_pairs [COLOR_PAIRS*2];	//global to acurses only

static SYSVAR *sv;
char _curse_buf[512];

void start_color() {
	// This routine is supposed to define the global variable for number of colours and number of colour pairs
	vdp_set_text_colour( COLOR_WHITE );
	vdp_set_text_colour( COLOR_BLACK + COLOR_BG);
};

int erase(void){
    vdp_clear_screen(); return true;
};

int clear(void){
    vdp_clear_screen(); return true;
};

int clearok(int win, bool state){
    vdp_clear_screen();
    return true;
};

int initscr() {
	sv = vdp_vdu_init();
    vdp_mode( 3 );	//Mode 3 is 80x30
    vdp_clear_screen();
	vdp_get_scr_dims( true );
    vdp_logical_scr_dims( false );
    return true;
};

int getcury(int win)
{
	return sv->cursorY;
};

int getcurx(int win)
{
	return sv->cursorX;
};

//#define sysvar_cursorX		    0x07    // 1: Cursor X position
//#define sysvar_cursorY		    0x08    // 1: Cursor Y position
//we might need this:	vdp_get_scr_dims( true );

int getmaxy(int win)
{
//	vdp_get_scr_dims( true );
	return sv->scrRows;
};

int getmaxx(int win)
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
	return false;	// yes the VDU is capable but not done yet.
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

//int vw_printw(WINDOW *win, const char *fmt, va_list varglist);

int vw_printw(int win, const char *fmt,...)
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


// terminal doesn't support orig_colours
int use_default_colors(void)
{
	return false;
};

int refresh(void)
{
	return true;
};

//dump font and reset text mode to bright white on black. NOTE: assumes video mode has at least 16 colours
int endwin(void)
{
	vdp_reset_system_font();
	vdp_set_text_colour( COLOR_WHITE + 8 );
	vdp_set_text_colour( COLOR_BLACK + COLOR_BG);
	return true;
};

//if we always return true, then endwin should be executed
int isendwin(void)
{
	return true;
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
			}
	else {vdp_set_text_colour(COLOR_WHITE + bold );};
	return true;
};

// I'm currently turning EVERYTHING off, which is not correct, but will do for now.
int attroff(int attrs)
{
	vdp_set_text_colour( COLOR_WHITE );
	vdp_set_text_colour( COLOR_BLACK + COLOR_BG);
	return true;
};

int attr_get (attr_t *attr, int *n, int *m)
{
	return false;
};

//int wmove(WINDOW *win, int y, int x);
int wmove(int win, int y, int x)
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
			vdp_set_text_colour( f + bold);
			}
	else {vdp_set_text_colour(COLOR_WHITE + bold );};
	putchar(ch);
	return true;
};

int mvaddch(int y, int x, const chtype ch)
{
    vdp_cursor_tab( x, y);
    return addch(ch);
};

int noecho (void)
{
	return false;
};

int nonl (void)
{
	return false;
};

// The halfdelay routine is used for half-delay mode, which is similar to cbreak mode in that characters typed by the user are immediately available to the program. However, after blocking for tenths tenths of seconds, ERR is returned if nothing has been typed. The value of tenths must be a number between 1 and 255. Use nocbreak to leave half-delay mode.

int halfdelay(int tenths)
{
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
    addstr(str);
    return true;
};

int beep(void)
{
	putchar(7);
	return true;
};

int keypad(int win, bool bf)
{
	return false;
};
