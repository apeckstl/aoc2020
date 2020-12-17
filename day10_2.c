#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 98 

int cmpfunc (const void * a, const void * b) { return (*(int*)a - *(int*)b); }
unsigned long long int numValidPaths(int *, unsigned long long int *, int);

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	int *list = calloc(1,NUM_ROWS*sizeof(int) + 1);
	unsigned long long int *memorize = calloc(NUM_ROWS+1, sizeof(unsigned long long int));
	int highestJoltage = 0;
	
	for (int i = 1; i <= NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		if (atoi(line) > highestJoltage)
			highestJoltage = atoi(line);
		list[i] = atoi(line);
	}

	qsort(list, NUM_ROWS+1, sizeof(int), cmpfunc);

	unsigned long long int answer = numValidPaths(list, memorize, 0);

	printf("Answer = %llu\n", answer);

	free(line);
	free(list);
	free(memorize);

	return 0;
}

// finds number of valid paths starting from given index

unsigned long long int numValidPaths(int *list, unsigned long long int *memorize, int idx) {
	if (memorize[idx] > 0) {
		return memorize[idx];
	}
	// last element in list, so 1 valid path
	if (idx == NUM_ROWS) {
		return 1;
	}
	unsigned long long int valid = 0;

	for (int i = 1; i < 4; i++) {
		if (idx + i <= NUM_ROWS && list[idx+i] - list[idx] < 4) {
			valid += numValidPaths(list,memorize,idx+i);
		}
	}

	memorize[idx] = valid;
	return valid;
}



