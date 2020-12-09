#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 594

typedef struct bag {
	char *name;
	int numParents;
	int *parents;
	int contains;
} Bag;

void addToBags(char *, Bag *);
void addParent(char *, int, Bag *);
void bagsContainingBag(int, Bag *);


int main(int argc, char **argv) {
	FILE *fp;
	Bag *gbags = calloc(NUM_ROWS, sizeof(Bag));

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	int goldidx = -1;
	for (int i = 0; i < NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		addToBags(line, &gbags[i]);
		if (strcmp(gbags[i].name,"shinygold") == 0)
			goldidx = i;
	}

	fseek(fp, 0, SEEK_SET);
	
	for (int i = 0; i < NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		addParent(line, i, gbags);
	}

	bagsContainingBag(goldidx, gbags);
	int numBags = 0;
	
	free(line);
	fclose(fp);

	for (int i = 0; i < NUM_ROWS; i++) {
		if (gbags[i].contains == 1) {
			numBags++;
		}
		free(gbags[i].name);
		free(gbags[i].parents);
	}

	free(gbags);

	printf("Answer = %d\n", numBags-1);

	return 0;
}

void addToBags(char *line, Bag *newBag) {
	char *name = malloc(15);
	int *parents = malloc(sizeof(int) * 50);
	sscanf(line, "%s", name);

	newBag->name = name;
	newBag->parents = parents;
}

// given a bag name, find the index in gbags
int findBag(char *name, Bag *gbags) {
	for (int i = 0; i < NUM_ROWS; i++) {
		if (strcmp(gbags[i].name, name) == 0)
			return i;
	}
	return -1;
}

void addParent(char *line, int pidx, Bag *gbags) {
	int charsRead = 0;
	char *parentName = malloc(15);
	char *childName = malloc(15);
	int numBags = 0;
	sscanf(line, "%s : %n",parentName, &charsRead);
	line += charsRead;

	while (sscanf(line, "%d %s%n", &numBags, childName, &charsRead) == 2) {
		int childidx = findBag(childName, gbags);
		Bag *child = &gbags[childidx];
		child->parents[child->numParents] = pidx;
		child->numParents++;
		line += charsRead;
	}

	free(parentName);
	free(childName);

}

void bagsContainingBag(int idx, Bag *gbags) {
	Bag *bag = &gbags[idx];
	bag->contains = 1;
	for (int i = 0; i < bag->numParents; i++) {
		bagsContainingBag(bag->parents[i], gbags);
	}
}
