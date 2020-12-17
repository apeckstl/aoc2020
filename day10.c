#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 98

int nextAdapter(int *, int, int *);

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	int *list = calloc(1,NUM_ROWS*sizeof(int));
	int highestJoltage = 0;
	
	for (int i = 0; i < NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		if (atoi(line) > highestJoltage)
			highestJoltage = atoi(line);
		list[i] = atoi(line);
	}
	
	int threes = 1;
	int ones = 0;
	int result = -1;
	int currentJoltage = 0;
	int nextJoltage = 0;

	while ((result = nextAdapter(list, currentJoltage, &nextJoltage)) != 0) {
		currentJoltage = nextJoltage;
		if (result == 1)
			ones++;
		else
			threes++;
	}


	printf("Answer = %d\n", ones*threes);

	return 0;
}

// returns difference (1 or 3) between the current adapter
// and the next one, and stores the joltage of the new
// one in joltage. Sets the index of the adapter it found
// to -1
int nextAdapter(int *list, int current, int *joltage) {
	int threeIdx = -1;

	for (int i = 0; i < NUM_ROWS; i++) {
		//printf("element: %d ", list[i]);
		if (list[i] - current == 1) {
			//printf(" and it's one greater than current\n");
			*joltage = list[i];
			list[i] = -1;
			return 1;
		} else if (list[i] - current == 3) {
			threeIdx = i;
		}
	}
	if (threeIdx != -1) {
		*joltage = list[threeIdx];
		list[threeIdx] = -1;
		//printf(" and it's 3 greater than current\n");
		return 3;
	}
	return 0;
}
