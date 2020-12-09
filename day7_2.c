#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 594

typedef struct bag {
	char *name;
	int numChildren;
	int *children;
	int *eachChild;
	int contains;
} Bag;

void addToBags(char *, Bag *);
void addChildren(char *, int, Bag *);
int bagContains(int, Bag *);


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
		addChildren(line, i, gbags);
	}

	int numBags = bagContains(goldidx, gbags);
	
	free(line);
	fclose(fp);

	for (int i = 0; i < NUM_ROWS; i++) {
		free(gbags[i].name);
		free(gbags[i].children);
		free(gbags[i].eachChild);
	}

	free(gbags);

	printf("Answer = %d\n", numBags);

	return 0;
}

void addToBags(char *line, Bag *newBag) {
	char *name = malloc(15);
	int *children = malloc(sizeof(int) * 4);
	int *eachChild = malloc(sizeof(int) * 4);
	sscanf(line, "%s", name);

	newBag->name = name;
	newBag->children = children;
	newBag->eachChild = eachChild;
}

// given a bag name, find the index in gbags
int findBag(char *name, Bag *gbags) {
	for (int i = 0; i < NUM_ROWS; i++) {
		if (strcmp(gbags[i].name, name) == 0)
			return i;
	}
	return -1;
}

void addChildren(char *line, int pidx, Bag *gbags) {
	int charsRead = 0;
	char *parentName = malloc(15);
	char *childName = malloc(15);
	int numBags = 0;
	Bag *bag = &gbags[pidx];
	sscanf(line, "%s : %n",parentName, &charsRead);
	line += charsRead;

	while (sscanf(line, "%d %s%n", &numBags, childName, &charsRead) == 2) {
		int childidx = findBag(childName, gbags);
		bag->children[bag->numChildren] = childidx;
		bag->eachChild[bag->numChildren] = numBags;
		bag->numChildren++;
		line += charsRead;
	}

	free(parentName);
	free(childName);

}

int bagContains(int idx, Bag *gbags) {
	Bag *bag = &gbags[idx];
	int number = 0;
	for (int i = 0; i < bag->numChildren; i++) {
		number += bag->eachChild[i];
		number += bag->eachChild[i] * bagContains(bag->children[i], gbags);
	}

	return number;
}
