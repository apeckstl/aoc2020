#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int numFields;
int N;
char **ruleNames;

void insertRule(int,char*,unsigned int*);
int numValidFields(unsigned int);
void updateClues(char*,unsigned int*,unsigned int*);
void elimination(unsigned int *);

int main(int argc, char **argv) {
	int address, value;
	FILE *fp;
	numFields = 20;
	N = atoi(argv[2]);
	unsigned int *rules = calloc(1000,sizeof(unsigned int));
	ruleNames = calloc(20, sizeof(char*));
	int myTicket[20];

	// set up clues. for each rule "clue" we have a bitmap
	// of the fields it is valid for (a 1 indicates an invalid field)
	unsigned int *clues = calloc(20,sizeof(unsigned int));

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	// set up rules
	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < numFields; i++) {
		getline(&line, &charactersRead, fp);
		insertRule(i, line, rules);
	}

	// for each ticket, update the clues
	for (int i = 21; i <= N; i++) {
		getline(&line, &charactersRead, fp);
		char *place = line;
		// special case my ticket
		if (i == 21) {
			int bytesRead;
			for (int j = 0; j < numFields; j++) {
				sscanf(place, "%d,%n", &(myTicket[j]), &bytesRead);
				place += bytesRead;
			}
			continue;
		}
		updateClues(line, rules, clues);
	}

	// after elimination, each rule will have only one possible field that is valid
	elimination(clues);


	// multiply my ticket values for the first 6 fields
	long long int product = 1;
	for (int i = 0; i < 6; i++) {
		printf("%s: field ", ruleNames[i]);
		unsigned int mask = 1;
		for (int j = 0; j < 20; j++) {
			if ((mask & clues[i]) == 0) {
				printf("%d = %d\n", j, myTicket[j]);
				product *= myTicket[j];
				break;
			}
			mask = mask << 1;
		}
	}
	printf("\n");

	for (int i = 0; i < numFields; i++) {
		free(ruleNames[i]);
	}
	free(ruleNames);
	free(clues);
	free(rules);

	free(line);
	fclose(fp);

	printf("Answer = %lld\n", product);
	return 0;
}

// index goes from 0 to 19, one for each rule. I parse both ranges for
// the rule given by 'line' and at each valid value, I OR the existing
// value there, so that each value has a bitmap of rules that allow that
// value
void insertRule(int index, char *line, unsigned int *rules) {
	char *ruleName = calloc(1, 20*sizeof(char));
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
	ruleNames[index] = ruleName;
}

void updateClues(char *ticket, unsigned int *rules, unsigned int *clues) {
	int value;
	int sum;
	int bytesRead;
	char *place = ticket;

	// for each field on the ticket check to make sure it fulfills at least one rule
	for (int i = 0; i < numFields; i++) {
		sscanf(place, "%d,%n", &value, &bytesRead);
		if (rules[value] == 0) {
			return;
		}
		place += bytesRead;
	}

	place = ticket;
	// for each field on the ticket
	unsigned int fieldMask = 1;
	for (int i = 0; i < numFields; i++) {
		sscanf(place, "%d,%n", &value, &bytesRead);
		unsigned int mask = 1;
		// for each bit in the rule bitmap for that
		// value check if field "i" is valid for that rule
		// if not, we want the clue for that rule to be 0
		// at field i. we do that by using the outer mask 
		for (int j = 0; j < numFields; j++) {
			if ((mask & rules[value]) == 0) {
				// there is an invalid rule for this field
				clues[j] |= fieldMask;
			}
			mask = mask << 1;
		}
		place += bytesRead;
		fieldMask = fieldMask << 1;
	}
}

// loop over the clues, looking for one with only one valid field
// remove that field from all other clues
// stop loop when a full iteration finds no clues to check
void elimination(unsigned int *clues) {

	int cluesToCheck[20];

	int fieldsSet = 0;

	while (fieldsSet < 20) {
		for (int i = 0; i < 20; i++) {
			// this clue is set
			if (cluesToCheck[i] == -1)
				continue;
			
			unsigned int mask = 1;
			int matched = 0;
			int match;
			// j is each field bit in the clue
			for (int j = 0; j < 20; j++) {
				if ((mask & clues[i]) == 0) {
					matched++;
					match = j;
				}
				mask = mask << 1;
			}
			// this one only has one valid rule
			if (matched == 1) {
				cluesToCheck[i] = -1;
			} else {
				continue;
			}

			// j is each other clue
			for (int j = 0; j < 20; j++) {
				if (j != i)
					clues[j] |= (1 << match);
			}
			fieldsSet++;
			break;

		}

	}

}
