#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 454

int countAffirmative(int *, int);
int getGroupCount(char *);

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	int sum = 0;
	for (int i = 0; i < NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		sum += getGroupCount(line);
	}

	free(line);
	fclose(fp);

	printf("Answer = %d\n", sum);

	return 0;
}

// given a 26 int array, return the number of 1's
int countAffirmative(int *positions, int groupSize) {
	int sum = 0;

	for (int i = 0; i < 26; i++) {
		if (positions[i] == groupSize)
			sum++;
	}

	return sum;

}

int getGroupCount(char *line) {
	int *groupAnswers = calloc(1,26*sizeof(int));
	int numChars = 0;
	char person[27];
	int groupSize = 0;

	while (sscanf(line, " %s%n", person, &numChars) == 1) {
		for (int i = 0; i < strlen(person); i++) {
			groupAnswers[(int)(person[i]) - 97]++;
		}
		line += numChars;
		groupSize++;
	}

	int groupCount = countAffirmative(groupAnswers, groupSize);
	free(groupAnswers);

	return groupCount;
}
