char pipes[PIPEWIDTH];

void initColors()
{
	start_color();
	use_default_colors();
	init_pair(PIPECOLOR, COLOR_WHITE,  COLOR_CYAN);
	init_pair(BIRDCOLOR, COLOR_YELLOW, COLOR_DEFAULT);
	init_pair(DEADCOLOR, COLOR_RED,    COLOR_DEFAULT);
}

void initPipes()
{
	int i;

	for (i = 0; i < PIPEWIDTH; i++)
		pipes[i] = '|';
}

void drawPipes(int x, int y)
{
	int i, j, v;

	if (x >= WINDOWWIDTH || (x + PIPEWIDTH) < 0 || y == 0)
		return;

	attron(COLOR_PAIR(PIPECOLOR));

	if (x < 0) {
		v = 0 - x;
		j = 0;
	}
	else {
		v = x + PIPEWIDTH - WINDOWWIDTH;
		j = x;
		if (v <= 0)
			v = 0;
	}

	if (y - GAP > 0) {
		for (i = 0; i < (y - GAP); i++)
			mvprintw(i, j, "%s", &pipes[v]);
	}
	
	for (i = y; i < (WINDOWHEIGHT - 1); i++)
		mvprintw(i, j, "%s", &pipes[v]);

	attroff(COLOR_PAIR(PIPECOLOR));
}

void drawBird(int y, char state)
{
	if (state & 2) {
		if (y < 0)
			y = 0;
		if (y > (WINDOWHEIGHT - 2))
			y = WINDOWHEIGHT -2;

		attron(COLOR_PAIR(DEADCOLOR));
		mvprintw(y, BIRDX, "~>>0~");
		attroff(COLOR_PAIR(DEADCOLOR));
	}
	else {
		attron(COLOR_PAIR(BIRDCOLOR));
		if (state & 1) {
			mvprintw(y - 1, BIRDX, "\\\\");
			mvprintw(y, BIRDX, "~>>0-");
		}
		else {
			mvprintw(y, BIRDX, "~>>0-");
			mvprintw(y + 1, BIRDX, "//");
		}
		attroff(COLOR_PAIR(BIRDCOLOR));
	}
}

void drawScoreBoard(int score, int collisionFlag)
{
	if (collisionFlag) {
		mvprintw(WINDOWHEIGHT - 1, 1, "GAME-OVER. SCORE: %d. PLAY AGAIN? (Y/N): ", score);
	}
	else
		mvprintw(WINDOWHEIGHT - 1, 1, "SCORE: %d", score);
}
