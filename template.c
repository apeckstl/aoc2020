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

	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < N; i++) {
		getline(&line, &charactersRead, fp);
	}

	free(line);
	fclose(fp);

	printf("Answer = %d\n", 0);
	return 0;
}
