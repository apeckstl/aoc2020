#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}
	int numEntries = 200;
	
	// all integers in the file
	int *entries = calloc(numEntries, sizeof(int));

	// read all integers into file, checking against previous entries
	int i	 = 0;
	int done = 0;
	while (fscanf(fp, "%d", &(entries[i])) == 1) {
		for (int j = 0; j < i; j++) {
			if (entries[i] + entries[j] == 2020) {
				printf("%d + %d = 2020, product = %d\n",entries[i],entries[j],entries[i]*entries[j]);
				done = 1;
				break;
			}
			if (done)
				break;
		}
		i++;
	}

	return 0;
}
