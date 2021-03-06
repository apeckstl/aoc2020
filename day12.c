#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N;

typedef struct position {
	int x;
	int y;
	int headx;
	int heady;
} position;

void turn(position *, char, int);
void move(position *, char, int);

int main(int argc, char **argv) {
	FILE *fp;
	N = atoi(argv[2]);

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	position *ferry = calloc(1, sizeof(position));
	ferry->headx = 1;

	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < N; i++) {
		getline(&line, &charactersRead, fp);
		int d;
		char c;
		sscanf(line, "%c%d", &c, &d);
		if (c == 'L' || c == 'R') {
			turn(ferry, c, d);
		} else {
			move(ferry, c, d);
		}
	}

	free(line);
	fclose(fp);

	printf("Answer = %d\n", abs(ferry->x) + abs(ferry->y));

	free(ferry);
	return 0;
}

void move(position *ferry, char heading, int distance) {
	switch (heading) {
		case 'N':
			ferry->y += distance;
			break;
		case 'E':
			ferry->x += distance;
			break;
		case 'S':
			ferry->y -= distance;
			break;
		case 'W':
			ferry->x -= distance;
			break;
		case 'F':
			ferry->x += ferry->headx * distance;
			ferry->y += ferry->heady * distance;
			break;
	}
}

void turn(position *ferry, char direction, int degrees) {
	if (direction == 'L')
		degrees = 360 - degrees;

	switch (degrees) {
		case 180:
			ferry->headx *= -1;
			ferry->heady *= -1;
			break;
		case 90:
			if (ferry->headx == 0) {
				ferry->headx += ferry->heady;
				ferry->heady = 0;
			} else {
				ferry->heady -= ferry->headx;
				ferry->headx = 0;
			}
			break;
		case 270:
			if (ferry->headx == 0) {
				ferry->headx -= ferry->heady;
				ferry->heady = 0;
			} else {
				ferry->heady += ferry->headx;
				ferry->headx = 0;
			}
			break;
	}	
}
