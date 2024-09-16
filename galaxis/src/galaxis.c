/*
 * galaxis.c - a curses clone of the Macintosh game, Galaxis
 *
 * by Eric S. Raymond <esr@snark.thyrsus.com>, begun Sun Oct 23 1994
 *
 * SPDX-FileCopyrightText: (C) Eric S. Raymond <esr@thyrsus.com>
 * SPDX-License-Identifier: BSD-2-Clause
 */
#define _POSIX_SOURCE

#include <assert.h>
#include <ctype.h>
#include <acurses.h>
//#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <termios.h>
#include <time.h>
//#include <unistd.h>

/* miscellaneous constants */
#define CTRLC '\003' /* used as terminate command */
#define FF '\014'    /* used as redraw command */
#define ONBOARD(y, x) (x >= 0 && x < BWIDTH && y >= 0 && y < BDEPTH)

/* coordinate handling */
#define BWIDTH 20
#define BDEPTH 9
#define LIFEBOATS 4

/* board state flags */
#define S_LIFEBOAT 0x01 /* boat is here */
#define S_BOATSEEN 0x02 /* boat has been seen */
#define S_MARKED 0x04   /* place has been marked empty by player */
#define S_BLANKED 0x08  /* probe has been marked empty by player */
#define S_PROBED 0x10   /* probe has been dropped here */
#define S_REVERSE 0x20  /* this is a ray, mark it in rv */

/* display symbols */
#ifdef A_COLOR
#define LIFEBOAT ('L' | COLOR_PAIR(COLOR_RED))
#define UNKNOWN ('.' | COLOR_PAIR(COLOR_BLUE))
#define MARKED ('-' | COLOR_PAIR(COLOR_GREEN))
#define BLANKED ('o' | COLOR_PAIR(COLOR_GREEN))
#define HIDDEN ('X' | COLOR_PAIR(COLOR_MAGENTA))
#else
#define LIFEBOAT 'L'
#define UNKNOWN '.'
#define MARKED '-'
#define BLANKED 'o'
#define HIDDEN 'X'
#endif /* A_COLOR */

/* how to position us on player board */
#define PYBASE 3
#define PXBASE 7
#define PY(y) (PYBASE + (y))
#define PX(x) (PXBASE + ((x + 1) * 3))
#define pgoto(y, x) (void)move(PY(y), PX(x))
#define PXINV(x) ((((x)-PXBASE) / 3) - 1)
#define PYINV(y) ((y)-PYBASE)

/* other board locations */
#define COLWIDTH 80
#define PROMPTLINE 23              /* prompt line */
#define SYBASE PYBASE + BDEPTH + 2 /* move key diagram */
#define SXBASE 54
#define MYBASE SYBASE - 1 /* diagram caption */
#define MXBASE SXBASE
#define HYBASE SYBASE - 1 /* help area */
#define HXBASE PXBASE + 3
#define POSYBASE PYBASE + BDEPTH / 2

/* direction constants */
#define E 0
#define SE 1
#define S 2
#define SW 3
#define W 4
#define NW 5
#define N 6
#define NE 7
static int xincr[8] = {1, 1, 0, -1, -1, -1, 0, 1};
static int yincr[8] = {0, 1, 1, 1, 0, -1, -1, -1};

static unsigned char board[BDEPTH][BWIDTH], probes[BDEPTH][BWIDTH];
static int curx = (BWIDTH / 2);
static int cury = (BDEPTH / 2);

static int turn;       /* how many tries we've made */
static int boatsfound; /* number of boats found */
static bool revealed;  /* display hidden lifeboats */
static bool dohilite;  /* highlight those rays */

#define PR (void)addstr

static void outro(int sig)
/* end the game, either normally or due to signal */
{
	clear();
	(void)refresh();
	(void)resetterm();
	(void)echo();
	(void)endwin();
	exit(0);
}

static void intro(void)
/* initialize curses */
{
//	srand(time(0L) + getpid()); /* Kick the random number generator */
	srand(time(0L)); /* Kick the random number generator */

//	(void)signal(SIGINT, outro);
//	(void)signal(SIGINT, outro);
#ifdef SIGIOT
	(void)signal(SIGIOT, outro); /* for assert(3) */
#endif                               /* SIGIOT */
//	if (signal(SIGQUIT, SIG_IGN) != SIG_IGN) {
//		(void)signal(SIGQUIT, outro);
//	}

	(void)initscr();
#ifdef KEY_MIN
	keypad(stdscr, TRUE);
#endif /* KEY_MIN */
	(void)saveterm();
	(void)nonl();
	(void)cbreak();
	(void)noecho();

#ifdef A_COLOR
	start_color();

	init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
#endif /* A_COLOR */

#ifdef NCURSES_MOUSE_VERSION
	(void)mousemask(BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED,
	                (mmask_t *)NULL);
#endif /* NCURSES_MOUSE_VERSION */
}

/* VARARGS1 */
static void prompt(char *f, char *s) {
	/* print a message at the prompt line */
	(void)move(PROMPTLINE, 0);
	(void)clrtoeol();
	(void)printw(f, s);
	(void)refresh();
}

static void error(char *s) {
	(void)move(PROMPTLINE + 2, 0);
	(void)clrtoeol();
	if (s) {
		(void)addstr(s);
		(void)beep();
	}
}

static int rnd(int n) { return (((rand() & 0x7FFF) % n)); }

static void initgame(void) {
	int boatsplaced;

	memset(board, '\0', sizeof(char) * BWIDTH * BDEPTH);
	memset(probes, '\0', sizeof(char) * BWIDTH * BDEPTH);

	/* first, place the lifeboats */
	boatsplaced = 0;
	while (boatsplaced < LIFEBOATS) {
		int x = rnd(BWIDTH);
		int y = rnd(BDEPTH);

		if (!(board[y][x] & S_LIFEBOAT)) {
			board[y][x] |= S_LIFEBOAT;
			boatsplaced++;
		}
	}

	turn = boatsfound = 0;
	revealed = FALSE;
	dohilite = TRUE;
}

static int show(int i, int j) {
	int state = board[i][j];
	int hilite = ((state & S_REVERSE) ? A_REVERSE : A_NORMAL);

	if ((state & S_BOATSEEN)) {
		return (hilite | LIFEBOAT);
	} else if (revealed && (state & S_LIFEBOAT)) {
		return (hilite | HIDDEN);
	} else if ((state & S_BLANKED)) {
		return (hilite | BLANKED);
	} else if ((state & S_PROBED)) {
		return (hilite | ('0' + probes[i][j]));
	} else if ((state & S_MARKED)) {
		return (hilite | MARKED);
	} else {
		return (hilite | UNKNOWN);
	}
}

static void showmoves(int cury, int curx) {
	/* reverse-highlight or unhighlight the rays extending from a loxcation
	 */
	int n;
	int i, j;

	for (i = 0; i < BDEPTH; i++) {
		for (j = 0; j < BWIDTH; j++) {
			board[i][j] &= ~S_REVERSE;
		}
	}

	if (!dohilite) {
		return;
	}

	for (n = 0; n < 8; n++) {
		int ny = cury;
		int nx = curx;

		for (;;) {
			ny += yincr[n];
			nx += xincr[n];

			if (!ONBOARD(ny, nx) || board[ny][nx] == S_BOATSEEN) {
				break;
			}

			board[ny][nx] |= S_REVERSE;
		}
	}
}

static void redisplay(bool force) {
	/* redraw the main board */
	int i, j;

	if (force) {
		(void)clear();
		(void)mvaddstr(0, 35, "GALAXIS");
		(void)mvaddstr(1, 2,
		               "by Eric S. Raymond, based on Christian Franz's "
		               "original for the Macintosh");

		(void)mvprintw(HYBASE, HXBASE, "?       -- detailed help");
		(void)mvprintw(HYBASE + 1, HXBASE,
		               "p       -- drop or re-query a probe");
		(void)mvprintw(HYBASE + 2, HXBASE,
		               "<space> -- toggle the empty mark");
		(void)mvprintw(HYBASE + 3, HXBASE,
		               "o       -- toggle ray highlighting");
		(void)mvprintw(HYBASE + 4, HXBASE, "r       -- refresh screen");
		(void)mvprintw(HYBASE + 5, HXBASE, "q       -- quit this game");

		(void)mvaddstr(MYBASE, MXBASE, "Movement keys:");
		(void)mvaddstr(SYBASE, SXBASE, "y k u    7 8 9");
		(void)mvaddstr(SYBASE + 1, SXBASE, " \\|/      \\|/ ");
		(void)mvaddstr(SYBASE + 2, SXBASE, "h-+-l    4-+-6");
		(void)mvaddstr(SYBASE + 3, SXBASE, " /|\\      /|\\ ");
		(void)mvaddstr(SYBASE + 4, SXBASE, "b j n    1 2 3");
#ifdef A_UNDERLINE
		(void)mvaddstr(SYBASE + 5, SXBASE, "Arrow keys work.");
#endif /* A_UNDERLINE */
#ifdef NCURSES_MOUSE_VERSION
		if (tigetstr("Km")) {
			(void)mvaddstr(SYBASE + 6, PX(0),
			               "Your mouse may work.  If so, "
			               "double-click drops a probe");
			(void)mvaddstr(SYBASE + 7, PX(0),
			               "at the pointer and single-click "
			               "toggles an empty mark.");
		}
#endif /* NCURSES_MOUSE_VERSION */
	}

	(void)mvprintw(POSYBASE + 1, 0, "Tries: %d", turn);
	(void)mvprintw(POSYBASE + 2, 0, "Found: %d", boatsfound);

	for (i = 0; i < BDEPTH; i++) {
		for (j = 0; j < BWIDTH; j++) {
			pgoto(i, j);
			addch(show(i, j));
		}
	}

	pgoto(cury, curx);
	refresh();
}

static void help(void) {
	clear();

	(void)mvaddstr(0, 35, "GALAXIS\n\n");
	// clang-format off
    (void) printw(
	    "%d lifeboats from a crippled interstellar liner are adrift in this starfield.\n",
	    LIFEBOATS);
    (void) printw(
	    "To find them, you can place probes that look in all eight compass directions\n");
    (void) printw(
	    "and tell you how many lifeboats they see.  If you drop a probe directly on a\n");
    (void) printw(
	    "lifeboat it will be revealed immediately.  Your objective: find the lifeboats\n");
    (void) printw(
	    "as quickly as possible, before the stranded passengers run out of oxygen!\n\n");


    (void) printw(
    	    "To drop a probe, move the cursor to a cell and press the `p' key.  You can mark\n");
    (void) printw(
	    "any cell to show that you believe there is no lifeboat there by pressing the\n");
    (void) printw(
	    "spacebar.  You can unmark a cell in the same way.  Finally, you can quit with\n");
    (void) printw(
	    "`q', which will reveal all lifeboats.\n\n");

    (void) printw(
	    "When the game starts, all the locations you can see from a given spot are\n");
    (void) printw(
	    "reverse-video highlighted.  As you mark more of the board, this may become\n");
    (void) printw(
	    "visually confusing.  You can toggle the highlighting with the `o' key.\n\n");

    (void) printw(
	    "Sometimes, after you have found a lifeboat, doing `p' on a cell you've already\n");
    (void) printw(
	    "probed will place more known-empty marks (because you can see all the boats the\n");
    (void) printw(
	    "probe counted).  This second query will not cost you a turn.\n\n"); 

    (void) printw(
	    "Yes, this should have an X interface more like the Mac game.  So write one!");
	// clang-format on

	prompt("Press any key to continue...", "");
	getch();
	redisplay(TRUE);
}

static int getcoord(void) {
	/* keep taking keystrokes till we see a player move */
	int ny, nx, c;

	for (;;) {
		(void)mvprintw(POSYBASE, 0, "(%d, %c) ", curx, 'A' + cury);
		pgoto(cury, curx);
		curs_set(1);
		showmoves(cury, curx);
		redisplay(FALSE);

		switch (c = getch()) {
		case 'k':
		case '8':
#ifdef KEY_MIN
		case KEY_UP:
#endif /* KEY_MIN */
			ny = cury + BDEPTH - 1;
			nx = curx;
			break;
		case 'j':
		case '2':
#ifdef KEY_MIN
		case KEY_DOWN:
#endif /* KEY_MIN */
			ny = cury + 1;
			nx = curx;
			break;
		case 'h':
		case '4':
#ifdef KEY_MIN
		case KEY_LEFT:
#endif /* KEY_MIN */
			ny = cury;
			nx = curx + BWIDTH - 1;
			break;
		case 'l':
		case '6':
#ifdef KEY_MIN
		case KEY_RIGHT:
#endif /* KEY_MIN */
			ny = cury;
			nx = curx + 1;
			break;
		case 'y':
		case '7':
#ifdef KEY_MIN
		case KEY_A1:
#endif /* KEY_MIN */
			ny = cury + BDEPTH - 1;
			nx = curx + BWIDTH - 1;
			break;
		case 'b':
		case '1':
#ifdef KEY_MIN
		case KEY_C1:
#endif /* KEY_MIN */
			ny = cury + 1;
			nx = curx + BWIDTH - 1;
			break;
		case 'u':
		case '9':
#ifdef KEY_MIN
		case KEY_A3:
#endif /* KEY_MIN */
			ny = cury + BDEPTH - 1;
			nx = curx + 1;
			break;
		case 'n':
		case '3':
#ifdef KEY_MIN
		case KEY_C3:
#endif /* KEY_MIN */
			ny = cury + 1;
			nx = curx + 1;
			break;
#ifdef NCURSES_MOUSE_VERSION
		case KEY_MOUSE: {
			MEVENT myevent;

			getmouse(&myevent);

			if (myevent.y >= PY(0) && myevent.y <= PY(BDEPTH) &&
			    myevent.x >= PX(0) && myevent.x <= PX(BWIDTH)) {
				nx = PXINV(myevent.x);
				ny = PYINV(myevent.y);
				if (myevent.bstate & BUTTON1_DOUBLE_CLICKED) {
					ungetch('p');
				} else if (myevent.bstate & BUTTON1_CLICKED) {
					ungetch(' ');
				}
			} else {
				beep();
				continue;
			}
		} break;
#endif /* NCURSES_MOUSE_VERSION */
		case 'r':
		case FF: /* undocumented */
			nx = curx;
			ny = cury;
			(void)clearok(stdscr, TRUE);
			(void)refresh();
			break;
		default:
			return (c);
		}

		curx = nx % BWIDTH;
		cury = ny % BDEPTH;
	}
}

static void count_lifeboats(int cury, int curx, int *total, int *visible)
/* count lifeboats visible from here */
{
	register int n, tot, vis;

	*total = *visible = tot = vis = 0;
	for (n = 0; n < 8; n++) {
		int ny = cury;
		int nx = curx;

		for (;;) {
			ny += yincr[n];
			nx += xincr[n];

			if (!ONBOARD(ny, nx)) {
				break;
			}

			if (board[ny][nx] & S_BOATSEEN) {
				++tot;
				++vis;
				break; /* stop counting in this direction */
			}

			if (board[ny][nx] & S_LIFEBOAT) {
				++tot;
				break; /* stop counting in this direction */
			}
		}
	}

	*total = tot;
	*visible = vis;
}

static void auto_mark(int cury, int curx)
/* automatically place marks on all rays */
{
	int n;

	for (n = 0; n < 8; n++) {
		int ny = cury;
		int nx = curx;

		for (;;) {
			ny += yincr[n];
			nx += xincr[n];

			if (!ONBOARD(ny, nx) || (board[ny][nx] & S_BOATSEEN)) {
				break;
			}

			if (!(board[ny][nx] & ~S_REVERSE)) {
				board[ny][nx] |= S_MARKED;
			}
		}
	}

	board[cury][curx] |= S_BLANKED;
}

static void drop_probe(int cury, int curx)
/* drop a probe on the given spot */
{
	if (board[cury][curx] & S_BOATSEEN) {
		prompt("This lifeboat is already visible.", "");
		beep();
	} else if (board[cury][curx] & S_LIFEBOAT) {
		board[cury][curx] |= S_BOATSEEN;
		prompt("You found a lifeboat!", "");
		beep();
		turn++;
		boatsfound++;
	} else {
		int visibles, total;

		count_lifeboats(cury, curx, &total, &visibles);

		if (!(board[cury][curx] & S_PROBED)) {
			board[cury][curx] |= S_PROBED;
			probes[cury][curx] = total;
			turn++;
		} else {
			prompt("You have already probed this location.", "");
		}

		/* auto-mark all rays when all lifeboats on them are visible */
		if (probes[cury][curx] == visibles) {
			auto_mark(cury, curx);
		}
	}
}

static bool domove(void)
/* accept a player move */
{
	switch (getcoord()) {
	case '?':
		help();
		break;

	case ' ':
		if (board[cury][curx] & S_PROBED) {
			board[cury][curx] ^= S_BLANKED;
		} else {
			board[cury][curx] ^= S_MARKED;
		}
		break;

	case 'o':
		dohilite = !dohilite;
		redisplay(TRUE); /* TRUE avoids color loss when we toggle off */
		break;

	case 'p':
		drop_probe(cury, curx);
		break;

	case 'q':
		revealed = TRUE;
		redisplay(FALSE);
		return (FALSE);

	default:
		prompt("Sorry, I don't recognize that command.", "");
		break;
	}

	return (TRUE);
}

static bool allmarked(void)
/* does user believe he knowa the whole board state */
{
	bool yes = TRUE;
	int i, j;

	for (i = 0; i < BDEPTH; i++) {
		for (j = 0; j < BWIDTH; j++) {
			if (!(board[i][j] || probes[i][j])) {
				yes = FALSE;
			}
		}
	}
	return (FALSE);
}

static int confirm(void) {
	int ch;

	(void)refresh();
	for (;;) {
		ch = getch();
		if (islower(ch)) {
			ch = toupper(ch);
		}
		if (ch == CTRLC) {
			outro(0);
		}
		if (ch == 'N' || ch == 'Y') {
			(void)addch((chtype)ch);
			(void)refresh();
			return (ch == 'Y');
		}
	}
}

int main(argc, argv)
int argc;
char *argv[];
{
	intro();
	do {
		initgame();
		redisplay(TRUE);

		while (boatsfound < LIFEBOATS && !allmarked() && domove()) {
			continue;
		}

		redisplay(FALSE); /* force found count to be updated */
		prompt("Want to play again [yn]? ", "");
	} while (confirm());
	outro(0);
	/*NOTREACHED*/
}

/* galaxis.c ends here */
