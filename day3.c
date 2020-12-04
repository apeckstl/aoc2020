#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;
    int patternWidth = 31;
    int mapHeight = 323;
    char pattern[mapHeight][patternWidth];
    int slope = 3;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < patternWidth; j++) {
            if ((pattern[i][j] = fgetc(fp)) == '\n') 
                pattern[i][j] = fgetc(fp);
        }
    }

    int x = 0;
    int y = 0;
    int trees = 0;

    while (y < mapHeight) {
        if (pattern[y][x] == '#')
            trees++;
        y++;
        x = (x + slope) % patternWidth;
    }

    printf("Trees encountered: %d\n", trees);
	
	return 0;
}
