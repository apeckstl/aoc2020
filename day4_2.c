#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <time.h>

#define NUM_PASSPORTS 254

int checkPassport(char *);

int main(int argc, char **argv) {
	clock_t start, end;
    double cpu_time_used;
     
    start = clock();
    FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

	int validPassports = 0;
	size_t charactersRead = 0;
	char *line = NULL;
	for (int i = 0; i < NUM_PASSPORTS; i++) {
		getline(&line, &charactersRead, fp);
		if (checkPassport(line))
			validPassports++;
	}
	free(line);

	fclose(fp);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	
	printf("Number of valid passports = %d, time = %f secs\n",validPassports, cpu_time_used);

	return 0;
}

// checks that a given 3 characters matches a required field
int checkFieldRequiredAndValid(char *field, char *value) {
	
	if (strcmp(field,"byr") == 0) {
		if (atoi(value) <= 2002 && atoi(value) >= 1920) 
			return 1;
		
		return 0;
	} else if (strcmp(field,"iyr") == 0) {
		if (atoi(value) <= 2020 && atoi(value) >= 2010)
			return 1;
		return 0;
	} else if (strcmp(field,"eyr") == 0) {
		if (atoi(value) <= 2030 && atoi(value) >= 2020)
			return 1;
		return 0;
	} else if (strcmp(field,"hgt") == 0) {
		char units[3];
		int height = 0;
		if (sscanf(value,"%d%s",&height,units) != 2)
			return 0;
		if (strcmp(units,"cm") == 0 && height <= 193 && height >= 150)
			return 1;

		if (strcmp(units,"in") == 0 && height <= 76 && height >= 59)
			return 1;
		return 0;
	} else if (strcmp(field,"hcl") == 0) {
		regex_t regex;
		regcomp(&regex,"#[0-9a-f]{6}",REG_EXTENDED);

		if (strlen(value) == 7 && regexec(&regex, value, 0, NULL, REG_EXTENDED) == 0) {
			regfree(&regex);
			return 1;
		}
		regfree(&regex);
		return 0;
	} else if (strcmp(field,"ecl") == 0) {
		char valid[7][4] = {"amb","blu","brn","gry","grn","hzl","oth"};
		for (int i = 0; i < 7; i++) {
			if (strcmp(value,valid[i]) == 0)
				return 1;
		}
		return 0;
	} else if (strcmp(field,"pid") == 0) {
		regex_t regex;
		regcomp(&regex,"[0-9]{9}",REG_EXTENDED);

		if (strlen(value) == 9 && regexec(&regex, value, 0, NULL, REG_EXTENDED) == 0) {
			regfree(&regex);
			return 1;
		}
		regfree(&regex);
		return 0;
	}
	
	return 0;
}

int checkPassport(char *passport) {

	char passportFields[NUM_PASSPORTS][3]; 
	int numRequiredFields = 0;
	char *myptr = passport;
	
	char field[4];
	char value[30];
	int numChars = 0;

	while (sscanf(myptr, " %[^:]:%s%n", field, value, &numChars) == 2) {
		if (checkFieldRequiredAndValid(field, value))
			numRequiredFields++;
		myptr += numChars;
	}


	if (numRequiredFields == 7)
		return 1;

	return 0;
}
