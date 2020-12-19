#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int N;

int main(int argc, char **argv) {
	FILE *fp;
	N = atoi(argv[2]);

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	int start = 0;
	char *c = malloc(5*sizeof(char));
	int busNo = 0;
	fscanf(fp, "%d\n", &start);
	printf("%d\n",start);
	int waitTime = start;
	while (fscanf(fp,"%[^,],", c) > 0) {
		if (strcmp(c,"x") != 0) {
			int sinceBus = start % atoi(c);
			int untilBus = atoi(c) - sinceBus;
			if (untilBus < waitTime) {
				waitTime = untilBus;
				busNo = atoi(c);
			}
			printf("remainder: %d bus #: %d until bus: %d\n", sinceBus, atoi(c), untilBus);
		}
	}

	fclose(fp);

	printf("Answer = %d\n", waitTime * busNo);
	return 0;
}
