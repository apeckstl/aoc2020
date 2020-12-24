#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int numFields;
int N;

void insertRule(int,char*,unsigned int*);
void printValidRules(unsigned int);
int numValidFields(unsigned int);
int sumInvalidFields(char*,unsigned int*);

int main(int argc, char **argv) {
	int address, value;
	FILE *fp;
	numFields = 20;
	N = atoi(argv[2]) - numFields;
	unsigned int *rules = calloc(1000,sizeof(unsigned int));

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < numFields; i++) {
		getline(&line, &charactersRead, fp);
		insertRule(i, line, rules);
	}

	int total;
	for (int i = 0; i < N; i++) {
		getline(&line, &charactersRead, fp);
		if (i == 0)
			continue;
		total += sumInvalidFields(line, rules);
	}

	free(line);
	fclose(fp);

	printf("Answer = %ld\n", total);
	return 0;
}

// index goes from 0 to 19, one for each rule. I parse both ranges for
// the rule given by 'line' and at each valid value, I OR the existing
// value there, so that each value has a bitmap of rules that allow that
// value
// departure location: 36-626 or 651-973
void insertRule(int index, char *line, unsigned int *rules) {
	char *ruleName = calloc(1, 20*sizeof(int));
	int leftStart;
	int leftEnd;
	int rightStart;
	int rightEnd;

	sscanf(line, "%[^:]: %d-%d or %d-%d", ruleName, &leftStart, &leftEnd, &rightStart, &rightEnd);

	unsigned int mask = 1 << index;
	for (int i = leftStart; i <= rightEnd; i++) {
		if (i > leftEnd && i < rightStart) {
			i = rightStart - 1;
			continue;
		}
		rules[i] |= mask;
	}
}

void printValidRules(unsigned int rule) {
	unsigned long int mask = 1;
	for (int i = 0; i < 20; i++) {
		if ((mask & rule) != 0) {
			printf("%lu ", i);
		}
		mask = mask << 1;
	}
	printf("\n");
}

int numValidRules(unsigned int rule) {
	int valid = 0;
	unsigned long int mask = 1;
	for (int i = 0; i < 20; i++) {
		if ((mask & rule) != 0) {
			valid++;
		}
		mask = mask << 1;
	}
	return valid;
}


int sumInvalidFields(char *ticket, unsigned int *rules) {
	int value;
	int sum;
	int bytesRead;

	for (int i = 0; i < 20; i++) {
		sscanf(ticket, "%d,%n", &value, &bytesRead);
		//printf("value: %d ", value);
		//printValidRules(rules[value]);
		if (numValidRules(rules[value]) == 0)
			sum += value;
		ticket += bytesRead;
	}
	return sum;
}
