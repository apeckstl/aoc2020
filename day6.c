#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 454

int countAffirmative(int *);
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
int countAffirmative(int *positions) {
	int sum = 0;

	for (int i = 0; i < 26; i++) {
		if (positions[i] == 1)
			sum++;
	}

	return sum;

}

int getGroupCount(char *line) {
	int *groupAnswers = calloc(1,26*sizeof(int));
	int numChars = 0;
	char person[27];

	while (sscanf(line, " %s%n", person, &numChars) == 1) {
		for (int i = 0; i < strlen(person); i++) {
			groupAnswers[(int)(person[i]) - 97] = 1;
		}
		line += numChars;
	}

	int groupCount = countAffirmative(groupAnswers);
	free(groupAnswers);

	return groupCount;
}
