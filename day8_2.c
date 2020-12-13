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

int beginExecution(Computer*, int);
void ins_acc(Computer*, int);
void ins_jmp(Computer*, int);

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

	for (int i = 0; i < NUM_ROWS; i++) {
		comp->acc = 0;
		comp->pc = 0;
		memset(comp->visited, 0, NUM_ROWS * sizeof(int));
		if (beginExecution(comp, i) == 0) {
			break;
		}
	}

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

int beginExecution(Computer *computer, int iteration) {
	char *ins = malloc(4);

	while (computer->pc < NUM_ROWS && computer->visited[computer->pc] != 1) {
		computer->visited[computer->pc] = 1;
		int arg = 0;

		sscanf(computer->program[computer->pc], "%s %d", ins, &arg);
		if (strcmp(ins, "acc") == 0) {
			ins_acc(computer, arg);
		} else if (strcmp(ins, "jmp") == 0) {
			// change to a nop
			if (computer->pc == iteration) {
				computer->pc++;
			} else
				ins_jmp(computer, arg);
		} else if (strcmp(ins, "nop") == 0) {
			// change to a jmp
			if (computer->pc == iteration) {
				ins_jmp(computer, arg);
			} else
				computer->pc++;
		}
	}

	free(ins);
	if (computer->pc >= NUM_ROWS) {
		return 0;
	}
	return 255;
}

void ins_acc(Computer *computer, int arg) {
	computer->acc += arg;
	computer->pc++;
}

void ins_jmp(Computer *computer, int arg) {
	computer->pc += arg;
}
