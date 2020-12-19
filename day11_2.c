#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FLOOR -1
#define EMPTY 0
#define OCCUPIED 1

int shouldBecomeOccupied(int gidx, int *seats);
int shouldBecomeEmpty(int gidx, int *seats);
int runOneIteration(int *seats, int *dest);
int countOccupied(int *seats);
int N;

int main(int argc, char **argv) {
	FILE *fp;
	N = atoi(argv[2]);
	int *seats1 = calloc(N*N, sizeof(int)); 
	int *seats2 = calloc(N*N, sizeof(int));

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < N; i++) {
		getline(&line, &charactersRead, fp);
		for (int j = 0; j < N; j++) {
			if (line[j] == '.')
				seats1[i*N + j] = FLOOR; // floor
			else if (line[j] == 'L')
				seats1[i*N + j] = EMPTY; // empty
			else if (line[j] == '#')
				seats1[i*N + j] = OCCUPIED;
		}
	}

	int *seats = seats1;
	int *dest = seats2;
	//int flag = 0;
	while (runOneIteration(seats, dest) != 1) { // while not stable
		if (seats == seats1) {
			seats = seats2;
			dest = seats1;
		} else {
			seats = seats1;
			dest = seats2;
		}
		/*for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (seats[i*N + j] == EMPTY)
					printf("L");
				else if (seats[i*N +j] == OCCUPIED)
					printf("#");
				else 
					printf(".");
			}
			printf("\n");
		}
		flag++;
		printf("\n");*/
		
	}

	free(line);
	fclose(fp);

	printf("Answer = %d\n", countOccupied(seats));

	return 0;
}



int countOccupied(int *seats) {
	int total = 0;

	for (int i = 0; i < N*N; i++) {
		if (seats[i] == OCCUPIED)
			total++;
	}

	return total;
}

int getRow(int gidx) {
	return gidx / N;
}

int getCol(int gidx) {
	return gidx % N;
}

int getGidx(int row, int col) {
	return row*N + col;
}

int firstSeatVisibleNorth(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (row > 0) {
		int indexToCheck = getGidx(--row,col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleSouth(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (row < N - 1) {
		int indexToCheck = getGidx(++row,col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleWest(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (col > 0) {
		int indexToCheck = getGidx(row,--col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleEast(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (col < N - 1) {
		int indexToCheck = getGidx(row,++col);
		//printf("%d ",col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleNortheast(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (row > 0 && col < N - 1) {
		int indexToCheck = getGidx(--row,++col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleSouthwest(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (row < N - 1 && col > 0) {
		int indexToCheck = getGidx(++row,--col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleSoutheast(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (row < N - 1 && col < N - 1) {
		int indexToCheck = getGidx(++row,++col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int firstSeatVisibleNorthwest(int gidx, int *seats) {
	int row = getRow(gidx);
	int col = getCol(gidx);
	while (row > 0 && col > 0) {
		int indexToCheck = getGidx(--row,--col);
		if (seats[indexToCheck] != FLOOR)
			return seats[indexToCheck];
	}
	return FLOOR;
}

int shouldBecomeOccupied(int gidx, int *seats) {
	if (firstSeatVisibleNorth(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleNortheast(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleEast(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleSoutheast(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleSouth(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleSouthwest(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleWest(gidx, seats) == OCCUPIED)
		return 0;
	if (firstSeatVisibleNorthwest(gidx, seats) == OCCUPIED)
		return 0;
	return 1;
}

int shouldBecomeEmpty(int gidx, int *seats) {
	int neighbors = 0;
	if (firstSeatVisibleNorth(gidx, seats) == OCCUPIED)
		neighbors++;
	if (firstSeatVisibleNortheast(gidx, seats) == OCCUPIED)
		neighbors++;
	if (firstSeatVisibleEast(gidx, seats) == OCCUPIED)
		neighbors++;
	if (firstSeatVisibleSoutheast(gidx, seats) == OCCUPIED)
		neighbors++;
	if (firstSeatVisibleSouth(gidx, seats) == OCCUPIED)
		neighbors++;
	if (neighbors >= 5)
		return 1;
	if (firstSeatVisibleSouthwest(gidx, seats) == OCCUPIED)
		neighbors++;
	if (neighbors >= 5)
		return 1;
	if (firstSeatVisibleWest(gidx, seats) == OCCUPIED)
		neighbors++;
	if (neighbors >= 5)
		return 1;
	if (firstSeatVisibleNorthwest(gidx, seats) == OCCUPIED)
		neighbors++;
	if (neighbors >= 5)
		return 1;
	return 0;

}

// returns 1 if stable. for each seat, if it needs to be changed,
// put new value in dest and set "stable = 0", and copy otherwise.
int runOneIteration(int *seats, int *dest) {
	int stable = 1;
	for (int i = 0; i < N*N; i++) {
		if (seats[i] == FLOOR) {
			dest[i] = FLOOR;
		} else if (seats[i] == EMPTY) {
			if (shouldBecomeOccupied(i, seats)) {
				dest[i] = OCCUPIED;
				stable = 0;
			} else {
				dest[i] = EMPTY;
			}
		} else {
			if (shouldBecomeEmpty(i, seats)) {
				dest[i] = EMPTY;
				stable = 0;
			} else {
				dest[i] = OCCUPIED;
			}
		}
	}

	return stable;
}
