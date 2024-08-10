struct pipeQueue
{
	int y[QLEN];
	double offset;
} pq;

double birdPos, birdVelocity, birdAcceleration, elapsedTime;
int birdY, count;
clock_t prevTS;

void updateElapsedTime()
{
	clock_t currTS;

	currTS = clock();
	elapsedTime = (double) (currTS - prevTS) / CLOCKS_PER_SEC;
	prevTS = currTS;
}

void initPhysics()
{
	prevTS = clock();
	birdAcceleration = 0.0;
	birdPos = (double) (WINDOWHEIGHT - 1) / 2.0;
	birdVelocity = 0.0;
	count = 0;
	updateElapsedTime();
	srand(time(NULL));
}

void updateBirdPos(char btnPressed)
{
	if (btnPressed && birdVelocity >= FLAPCONSTRAINT) {
		birdAcceleration = 0.0;
		birdVelocity = WINGPOWER;
	}
	else {
		birdAcceleration += GRAVITY * elapsedTime;
	}

	if (birdAcceleration > GRAVITY)
		birdAcceleration = GRAVITY; 

	birdVelocity += birdAcceleration * elapsedTime;
	birdPos += birdVelocity * elapsedTime;

	birdY = 0.5 + birdPos;
}

int getNewPipeSize()
{
	if (count == BREATHERPOS) {
		count = 0;
		return(0);
	}
	count++;
	return(rand() % (WINDOWHEIGHT - GAP) + GAP);
}

void initPipeQ()
{
	int i;

	pq.offset = PAD;

	for (i = 0; i < QLEN; i++)
		if (i * (2 * PAD + PIPEWIDTH) < (WINDOWWIDTH / 2))
			pq.y[i] = 0;
		else
			pq.y[i] = getNewPipeSize();
}

void updatePipeQ()
{
	int i;

	pq.offset += SPEED * elapsedTime;
	if (pq.offset < 0.0) {
		pq.offset = 2 * PAD + PIPEWIDTH;
		for (i = 1; i < QLEN; i++)
			pq.y[i - 1] = pq.y[i];
		pq.y[QLEN - 1] = getNewPipeSize();
	}
}

int detectCollision()
{
	int i, wingPos, pipePos;

	if (birdVelocity > 0.0)
		wingPos = birdY - 1;
	else
		wingPos = birdY + 1;

	if (wingPos < 0 || birdY < 0 || wingPos > (WINDOWHEIGHT - 2) || birdY > (WINDOWHEIGHT - 2))
		return(1);

	for (i = 0; i < QLEN; i++) {
		pipePos = (int)(pq.offset + 0.5) + i * (2 * PAD + PIPEWIDTH);
		
		if (pipePos > (BIRDX + 4))
			break;
		if ((pipePos + PIPEWIDTH) <= BIRDX)
			continue;
		if (pq.y[i] == 0)
			break;
		if ((birdY >= pq.y[i] && birdY <= (WINDOWHEIGHT - 2)) || (birdY >= 0 && birdY <= (pq.y[i] - GAP - 1)))
			return(1);

		if (pipePos > (BIRDX + 1))
			continue;
		if ((wingPos >= pq.y[i] && wingPos <= (WINDOWHEIGHT - 2)) || (wingPos >= 0 && wingPos <= (pq.y[i] - GAP - 1)))
			return(1);
	}

	return(0);
}
