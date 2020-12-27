#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "cubetable.h"

int N;
int minX;
int maxX;
int minY;
int maxY;
int minZ;
int maxZ;
int minW;
int maxW;

typedef struct cube {
	int x;
	int y;
	int z;
	int w;
	void *next;
} cube_t;

void printCube(cube_t*);
void printSpace(hashtable*);
void cycle(hashtable*);
int shouldBeAdded(int,int,int,int, hashtable*);
int countActive(hashtable*);

int main(int argc, char **argv) {
	FILE *fp;
	hashtable *ct = calloc(1, sizeof(ct));
	ht_init(ct, 2000);
	N = atoi(argv[2]);

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	// initialize
	minX = minY = minZ = minW = 0;
	maxX = maxY = N;
	maxZ = maxW = 1;
	for (int i = 0; i < N; i++) {
		getline(&line, &charactersRead, fp);
		for (int j = 0; j < N; j++) {
			//printf("(%d,%d,%d)\n", i, j, 0);
			if (line[j] == '.')
				continue;
			ht_put(ct, i, j, 0, 0);
		}
	}	
	//printSpace(ct);

	for (int i = 0; i < 6; i++) {
		cycle(ct);
		//printSpace(ct);
	}

	//printSpace(ct);

	free(line);
	fclose(fp);

	printf("Answer = %d\n", countActive(ct));
	return 0;
}

void printCube(cube_t *cube) {
	printf("cube: (%d,%d,%d)\n", cube->x, cube->y, cube->z);
}

void cycle(hashtable *ht) {
	// traverse bucket chain
	cube_t *toBeAdded = NULL;
	cube_t *toBeDeleted = NULL;
	
	for (int idx = 0; idx < ht->size; idx++) {
		bucket *current = ht->buckets[idx];
		while (current) {
			//printf("checking (%d,%d,%d)\n", current->x, current->y, current->z);
			// for each cube in the table
			// current is a bucket holding an active cube
			// go through each netoBeDeleted->x = current->x;
			// if neighbor in active cubes, increase neighbor count
			// else see if neighbor should be active and add neighbor to toBeAdded list
			// if neighbor count is not 2 or 3, add this cube to the toBeDeleted list
			int neighbors = 0;
			for (int i = current->x - 1; i <= current->x + 1; i++) {
				for (int j = current->y - 1; j <= current->y + 1; j++) {
					for (int k = current->z - 1; k <= current->z + 1; k++) {
						for (int l = current->w - 1; l <= current->w + 1; l++) {
							if (current->x == i && current->y == j && current->z == k && current->w == l)
								continue;
							if (ht_get(ht, i, j, k, l)) {
								// neighbor is active
								neighbors++;
							} else {
								// neighbor is inactive, should we add it?
								if (shouldBeAdded(i, j, k, l, ht)) {
									cube_t *temp = calloc(1, sizeof(cube_t));
									temp->x = i;
									temp->y = j;
									temp->z = k;
									temp->w = l;

									temp->next = toBeAdded;
									toBeAdded = temp;
								}
							}
						}
					}
				}
			}

			if (neighbors != 2 && neighbors != 3) {
				cube_t *temp = calloc(1, sizeof(cube_t));
				temp->x = current->x;
				temp->y = current->y;
				temp->z = current->z;
				temp->w = current->w;

				temp->next = toBeDeleted;
				toBeDeleted = temp;
			}

			if (current->next == NULL)
				break;
			current = current->next;
		}
	}

	while (toBeAdded) {
		//printf("adding (%d,%d,%d)\n", toBeAdded->x, toBeAdded->y, toBeAdded->z);
		cube_t *c = toBeAdded->next;
		ht_put(ht, toBeAdded->x, toBeAdded->y, toBeAdded->z, toBeAdded->w);
		free(toBeAdded);
		toBeAdded = c;
	}
	
	while (toBeDeleted) {
		cube_t *c = toBeDeleted->next;
		ht_del(ht, toBeDeleted->x, toBeDeleted->y, toBeDeleted->z, toBeDeleted->w);
		free(toBeDeleted);
		toBeDeleted = c;
	}

	minX--;
	minY--;
	minZ--;
	minW--;
	maxX++;
	maxY++;
	maxZ++;
	maxW++;

}

int countActive(hashtable *ht) {
	int count = 0;
	for (int idx = 0; idx < ht->size; idx++) {
		bucket *current = ht->buckets[idx];
		while (current) {
			count++;
			if (current->next == NULL)
				break;
			current = current->next;
		}
	}
	return count;
}

void printSpace(hashtable *ht) {
	for (int z = minZ; z < maxZ; z++) {
		printf("z layer %d\n", z);
		for (int x = minX; x < maxX; x++) {
			for (int y = minY; y < maxY; y++) {
				if (ht_get(ht,x,y,z,0) == 1) {
					printf("#");
				} else {
					printf(".");
				}
			}
			printf("\n");
		}
	}
}

// checks if an inactive cube should be activated
int shouldBeAdded(int x, int y, int z, int w, hashtable *ht) {
	int neighbors = 0;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			for (int k = z - 1; k <= z + 1; k++) {
				for (int l = w - 1; l <= w + 1; l++) { 
					if (!(x == i && y == j && z == k && w == l) && ht_get(ht, i, j, k, l) == 1) {
						//if (x == 0 && y == 0 && z == 0)
							//printf("our neighbor (%d,%d,%d) is active\n", i,j,k);

						neighbors++;
						if (neighbors > 3)
							return 0; //too many neighbors
					}
				}
			}
		}
	}
	if (neighbors == 3) // three neighbors
		return 1;

	return 0; // too few neighbors

}
