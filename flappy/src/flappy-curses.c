#include <stdlib.h>
#include <time.h>
#include <acurses.h>
#include "constants.h"
#include "physics.h"
#include "draw.h"

int main()
{
	int inputch, collisionFlag, i, pipePos, score;
//	int	screenshot = 0; for taking a cheeky screenshot
	double sumElapsed;
	char state;

	initscr();
	while(1) {
		noecho();
		cbreak();
		timeout(0);
		curs_set(0);

		initColors();
		initPhysics();
		initPipeQ();
		initPipes();
		score = 0;
		sumElapsed = 0.0;
		
		while(1) {
			inputch = wgetch(0);

			updateElapsedTime();
			updatePipeQ();
			updateBirdPos(inputch == ' ');

			collisionFlag = detectCollision(birdY);
			sumElapsed += elapsedTime;
			score = 2.5 * sumElapsed + 0.5;

			napms(20);	//screenshot++; while (screenshot>200){};
			erase();

			for (i = 0; i < QLEN; i++) {
				pipePos = (int)(pq.offset + 0.5) + i * (2 * PAD + PIPEWIDTH);
				drawPipes(pipePos, pq.y[i]);
			}

			state = collisionFlag * 2 + (birdVelocity > 0.0);
			drawBird(birdY, state);

			drawScoreBoard(score, collisionFlag);
			refresh();
			
			if (collisionFlag)
				break;
		}

		cbreak();
		timeout(-1);
		curs_set(1);

		while (1) {
			inputch = getch();
			if (inputch == 'n' || inputch == 'N' || inputch == 'y' || inputch =='Y')
				break;
			else
				continue;
		}

		if (inputch == 'y' || inputch == 'Y')
			continue;
		else
			break;
	}

	endwin();
	return(0);
}
