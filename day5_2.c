#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_PASSES 798

int getInt(char *,int);

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	int *seats = malloc(883*sizeof(int));
	int myId = 0;
	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < NUM_PASSES; i++) {
		getline(&line, &charactersRead, fp);
		int row = getInt(line, 7);
		int column = getInt(&line[7], 3);
		int id = 8 * row + column;
		seats[id] = 1;
	}

	int mode = 0;
	int i;
	for (i = 0; i < 883; i++) {
		if (seats[i] == 1)
			mode = 1;
		else if (mode == 1 && seats[i] == 0)
			break;
	}

	free(line);
	fclose(fp);

	printf("My boarding pass id = %d\n", i);

	return 0;
}

// gets row/col from 'FBFFBFF' notation
int getInt(char *pass, int numDigits) {
	int mask = (int)(pow(2.0,(double)numDigits)) / 2;
	int id = 0;

	for (int i = 0; i < numDigits; i++) {
		if (pass[i] == 'B' || pass[i] == 'R')
			id = id | mask;
		mask = mask >> 1;
	}

	return id;

}

