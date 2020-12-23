#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "hashtable.h"

#define N 7

/* globals */
int turn;

int main(int argc, char **argv) {
	int starting[N] = {6,4,12,1,20,0,16};

	hashtable *ht = calloc(1, sizeof(hashtable));
	ht_init(ht, 10000);

	for (int i = 1; i < N; i++) {
		ht_put(ht, starting[i-1], i);
	}

	int previous = starting[N-1];

	for (turn = N+1; turn < 30000001; turn++) {
		if (turn % 500000 == 0)
			printf("%d\n",turn);
		int p = ht_get(ht, previous);
		if (p == -1) {
			ht_put(ht, previous, turn-1);
			previous = 0;
		} else {
			ht_put(ht, previous, turn-1);
			previous = turn - p - 1;
		}
	}

	printf("Answer = %d\n", previous);

	ht_free(ht);

	return 0;
}

