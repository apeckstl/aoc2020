#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp;
    int patternWidth = 31;
    int mapHeight = 323;
    char pattern[mapHeight][patternWidth];
    int right[5] = {1,3,5,7,1};
    int down[5] = {1,1,1,1,2};
    int costs[5] = {0,0,0,0,0};

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < patternWidth; j++) {
            if ((pattern[i][j] = fgetc(fp)) == '\n') 
                pattern[i][j] = fgetc(fp);
        }
    }
    
    for (int route = 0; route < 5; route++) {
        int x = 0;
        int y = 0;
        int trees = 0;

        while (y < mapHeight) {
            if (pattern[y][x] == '#')
                trees++;
            y += down[route];
            x = (x + right[route]) % patternWidth;
        }   

        printf("Trees encountered: %d\n", trees);
        costs[route] = trees;
    }

    long total = 1;
    for (int i = 0; i < 5; i++) {
        total = total * costs[i];
    }

    printf("Product = %ld\n", total);
	
	return 0;
}
