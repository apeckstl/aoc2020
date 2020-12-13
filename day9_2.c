#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_ROWS 1000

int smolnbig(int, int, int *);

int main(int argc, char **argv) {
	FILE *fp;
	char *line;
	size_t charactersRead;
	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	int invalidNumber = 756008079;
	int *queue = calloc(1,1000*sizeof(int));
	int head = 0;
	int tail = 0;
	int sum = 0;
	int answer = 0;
						
	for (int i = 0; i < NUM_ROWS; i++) {
		getline(&line, &charactersRead, fp);
		sum += atoi(line);
		queue[tail] = atoi(line);
		tail++;
		while (sum > invalidNumber) {
			sum -= queue[head];
			head++;
		}
				
		if (sum == invalidNumber && head != tail) {
			//for (int i = head; i < tail - 1; i++)
			//	printf("%d + ", queue[i]);
			//printf("%d = %d\n", queue[tail-1], invalidNumber);
			answer = smolnbig(head, tail, queue);
			break;
		}

	}

	fclose(fp);
	free(queue);
	free(line);

	printf("Answer = %d\n", answer);

	return 0;
}

int smolnbig(int head, int tail, int *queue) {
	int smallest = queue[head];
	int largest = queue[head];

	for (int i = head + 1; i < tail; i++) {
		if (queue[i] > largest)
			largest = queue[i];
		else if (queue[i] < smallest)
			smallest = queue[i];
	}
	
	return smallest + largest;
}
