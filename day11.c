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
		printf("\n");
		*/
	}

	free(line);
	fclose(fp);

	printf("Answer = %d\n", countOccupied(seats));

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

int shouldBecomeOccupied(int gidx, int *seats) {
	int row = gidx / N;
	int col = gidx % N;

	for (int i = row-1; i <= row + 1; i++) {
		for (int j = col-1; j <= col + 1; j++) {
			if (i*N + j != gidx  && i < N && i >= 0 && j < N && j >= 0 && seats[i*N + j] == OCCUPIED)
				return 0;
		}
	}

	return 1;
}

int shouldBecomeEmpty(int gidx, int *seats) {
	int row = gidx / N;
	int col = gidx % N;
	int neighbors = 0;

	for (int i = row-1; i <= row + 1; i++) {
		for (int j = col-1; j <= col + 1; j++) {
			if (i*N + j != gidx && i < N && i >= 0 && j < N && j >= 0 && seats[i*N + j] == OCCUPIED)
				neighbors++;
		}
	}

	// if the seat at gidx has more than 4 occupied adjacent seats
	if (neighbors >= 4)
		return 1;

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

