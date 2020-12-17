#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 1000

void calculateSum(int idx, int new, int **preamble);

int main(int argc, char **argv) {
	FILE *fp;
	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	int (*preamble)[25] = malloc(25*25*sizeof(int));
	int setup[25];

	char *line = NULL;
	size_t charactersRead = 0;
	// create preamble
	for (int i = 0; i < 25; i++) {
		printf("row: %d\n",i);
		getline(&line, &charactersRead, fp);
		preamble[i][i] = 0;
		setup[i] = atoi(line);
	}

	// cheeky reuse of calculateSum, since the diagonal will be 0's
	for (int i = 0; i < 25; i++) {
		calculateSum(i, setup[i], preamble);
	}

	int spot = 0;
	int answer = 0;
	for (int i = 0; i < 975; i++) {	
		getline(&line, &charactersRead, fp);
		if (validNumber(atoi(line), preamble)) {
			calculateSum(spot, atoi(line), preamble);
			spot = (spot + 1) % 25;
		} else {
			answer = atoi(line);
			break;
		}
	}
	
	fclose(fp);

	printf("Answer = %d\n", answer);

	return 0;
}

void calculateSum(int idx, int new, int **pre) {
	// preamble is 25 rows, row number element in every row is the old number
	// we need to go through idx row, subtract the old number from each item
	// add the new number, then go through other rows, and subtract the old from
	// the idx element and add the new
	//
	int (*preamble)[25] = pre;
	int old = preamble[idx][idx];
	
	// replace in idx row first
	for (int i = idx; i < 25; i++) {
		preamble[idx][i] += (new - old);
	}

	// for each row, fix sum with the old element
	for (int i = 0; i < idx; i++) {
		if (i != idx)
			preamble[i][idx] += (new - old);
	}

}

int validNumber(int n, int **pre) {
	int (*preamble)[25] = pre;

	for (int i = 0; i < 25; i++) {
		for (int j = i; j < 25; j++) {
			if (n == preamble[i][j])
				return 1;
		}
	}

	return 0;
}
