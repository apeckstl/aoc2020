#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PASSPORTS 254

int checkPassport(char *);

int main(int argc, char **argv) {
	FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	int validPassports = 0;
	char *line = NULL;
	size_t charactersRead = 0;
	for (int i = 0; i < NUM_PASSPORTS; i++) {
		getline(&line, &charactersRead, fp);
		if (checkPassport(line))
			validPassports++;
	}

	free(line);
	fclose(fp);

	printf("Number of valid passports = %d\n",validPassports);

	return 0;
}

// checks that a given 3 characters matches a required field
int checkFieldRequired(char *field) {
	char requiredFields[8][4] = {"byr","iyr","eyr","hgt","hcl","ecl","pid"};

	for (int i = 0; i < 7; i++) {
		if (strcmp(field, requiredFields[i]) == 0)
			return 1;
	}
	
	return 0;
}

int checkPassport(char *passport) {
	char passportFields[NUM_PASSPORTS][3]; 
	int numRequiredFields = 0;
	
	char field[4];
	char value[30];
	int numChars = 0;

	while (sscanf(passport, " %[^:]:%s%n", field, value, &numChars) == 2) {
		if (checkFieldRequired(field))
			numRequiredFields++;
		passport += numChars;
	}

	if (numRequiredFields == 7)
		return 1;

	return 0;
}
