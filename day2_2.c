#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct policy {
	int low;
	int high;
	char letter;
} policy;

int checkPassword(policy *pol, char *password) {
	int len = strlen(password);
	int occurrences = 0;

	if (password[pol->low - 1] == pol->letter)
		occurrences++;

	if (password[pol->high - 1] == pol->letter)
		occurrences++;
	
	if (occurrences == 1)
		return 1;

	return 0;
}

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}
	int numPasswords = 1000;
	
	policy *pol = calloc(1,sizeof(policy));
	char *password = calloc(1,30);
	int validPasswords = 0;

	// read all passwords and policies from file
	for (int i = 0; i < numPasswords; i++) {
		fscanf(fp, "%d-%d %c: %s", &(pol->low), &(pol->high), &(pol->letter), password);
		if (checkPassword(pol, password)) 
			validPasswords++;
	}
	
	printf("Valid passwords = %d\n", validPasswords);

	return 0;
}
