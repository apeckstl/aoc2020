#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* globals */
int *turns;
int turn;

int findPreviousOccurrence(int);

int main(int argc, char **argv) {
	int starting[7] = {6,4,12,1,20,0,16};
	turns = calloc(2020, sizeof(int));
	memcpy(turns, starting, 7*sizeof(int));

	// turn represents the index of the number we are about to say
	for (turn = 7; turn < 2020; turn++) {
		int previous = findPreviousOccurrence(turns[turn-1]);
		turns[turn] = turn - previous - 1;
	}

	printf("Answer = %d\n", turns[2019]);
	free(turns);
	return 0;
}

// given the number that was last spoken, go through previous turns
// end <3
int findPreviousOccurrence(int lastSpoken) {
	for (int i = turn - 2; i >= 0; i--) {
		if (turns[i] == lastSpoken) {
			return i;
		}
	}
	return turn-1;
}
