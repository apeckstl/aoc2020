#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * Day 14 Part 1 accidentally got written over when I went to solve Part 2
 * I might eventually come back and resolve part 1 lol
 */


int N;
char *mask;
int numValues;
int size;
long int *addresses;
long int *values;

void transformAddressAndWrite(int, int);

int main(int argc, char **argv) {
	int address, value;
	FILE *fp;
	N = atoi(argv[2]);
	size = 10*N;

	mask = calloc(37, sizeof(char));
	addresses = calloc(size, sizeof(long int));
	values = calloc(size, sizeof(long int));

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < N; i++) {
		getline(&line, &charactersRead, fp);
		if (strncmp(line, "mask", 4) == 0) {
			strncpy(mask, line + 7, 36);
		} else {
			sscanf(line, "mem[%d] = %d", &address, &value);
			transformAddressAndWrite(address, value);
		}
	}
	
	free(line);
	fclose(fp);

	// final addition of all values
	long int total = 0;
	for (int i = 0; i < numValues; i++) {
		total += values[i];
	}

	printf("Answer = %ld\n", total);
	return 0;
}


// given an address in binary, transform to int
long int strToInt(char *address) {
	long int final = 0;
	long int position = (long int ) 1 << 35;
	for (int i = 0; i < 36; i++) {
		if (address[i] == '1')
			final += position;
		position /= 2;
	}
	return final;
}

// given an address, find the index where we keep that address' value
// blah blah bad big O notation
int findAddress(long int address) {
	if (numValues == size) {
		// reached end of array -> realloc!
		size += N;
		addresses = realloc(addresses, size * sizeof(long int));
		values = realloc(values, size * sizeof(long int));
	}
	for (int i = 0; i < numValues; i++) {
		if (addresses[i] == address)
			return i;
	}

	// never seen this dude before
	addresses[numValues] = address;
	return numValues++;
}

// given an address, find the first instance of 'X' and call writeValue on the address with a 0 and 1
// in that place. If there are no X'es, turn address into int and write to that value.
void writeValue(char *address, int value) {
	
	for (int i = 0; i < 36; i++) {
		if (address[i] == 'X') {
			char *left = calloc(36, sizeof(char));
			char *right = calloc(36, sizeof(char));
			strncpy(left, address, 36);
			strncpy(right, address, 36);
			// branch
			left[i] = '0';
			right[i] = '1';
			writeValue(left, value);
			writeValue(right, value);
			free(address);
			return;
		}
	}

	// leaf
	int idx = findAddress(strToInt(address));
	values[idx] = value;
	free(address);
}

// given an address, a value, and the lists of addresses and values, use the mask to transform
// the address into the masked form with floating bits. Then for each floating bit, call
// a recursive writeValue method to write to all permutations of the address
void transformAddressAndWrite(int add, int value) {
	long int position = (long int) 1 << 35;
	char *str = calloc(36, sizeof(char));
	for (int i = 0; i < 36; i++) {
        char original = (add & position) ? '1': '0';
		switch (mask[i]) {
			case 'X':
				str[i] = 'X';
				break;
			case '1':
				str[i] = '1';
				break;
			case '0':
				str[i] = original;
				break;
		}
		position /= 2;
	}
	writeValue(str, value);
	// <3
}

