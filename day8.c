#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 631

typedef struct computer {
	int acc;
	int pc;
	char **program;
	int *visited;
} Computer;

void beginExecution(Computer*);

int main(int argc, char **argv) {
	FILE *fp;
	Computer *comp = calloc(1,sizeof(Computer));
	char **lines = calloc(NUM_ROWS,sizeof(char *));
	int *visited = calloc(NUM_ROWS,sizeof(int));

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		char *instruction = malloc(charactersRead+1);
		strcpy(instruction,line);
		lines[i] = instruction;
	}
	comp->program = lines;
	comp->visited = visited;

	beginExecution(comp);

	free(line);
	fclose(fp);

	printf("Answer = %d\n", comp->acc);

	for (int i = 0; i < NUM_ROWS; i++)
		free(lines[i]);
	free(lines);
	free(visited);
	free(comp);

	return 0;
}

void beginExecution(Computer *computer) {
	int pc = computer->pc;
	char *ins = malloc(4);

	while (pc < NUM_ROWS && computer->visited[pc] != 1) {
		computer->visited[pc] = 1;
		int arg = 0;

		sscanf(computer->program[pc], "%s %d", ins, &arg);
		if (strcmp(ins, "acc") == 0) {
			computer->acc += arg;
			pc++;
		} else if (strcmp(ins, "jmp") == 0) {
			pc += arg;
		} else if (strcmp(ins, "nop") == 0) {
			pc++;
		}
	}

	computer->pc = pc;
	free(ins);
}
