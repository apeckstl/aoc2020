#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stringtable.h"
#include <regex.h>

int N;

regex_t single_char_regex;
regex_t multi_rule_regex;
regex_t or_regex;


int messageMatches(hashtable *ruleTable, char *message, int rule) {
    char *ruleString = ht_get(ruleTable, rule);

    if (regexec(&single_char_regex, ruleString, 0, NULL, 0) == 0) {
        if (message[0] == ruleString[1]) {
            
        }
    }
    
    

}

int main(int argc, char **argv)
{
    regcomp(&single_char_regex, "\"a|b\"", 0);
    regcomp(&multi_rule_regex, "([0-9]*| )*", 0);
    regcomp(&single_char_regex, "([0-9]*| )*\\|( |[0-9]*)*", 0);

    FILE *fp;

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

    int readAllRules = 0;

    hashtable *ruleTable = calloc(1, sizeof(hashtable));
	ht_init(ruleTable, 1000);

    while (!readAllRules) {
        char *line = NULL;
        size_t charactersRead = 0;

        getline(&line, &charactersRead, fp);

        if (strcmp(line, "\n") == 0) {
            readAllRules = 1;
        } else {
            char* ruleNo = strtok(line, ":");
            char* rule = strtok(NULL, "");

            ht_put(ruleTable, atoi(ruleNo), rule);
        }

    }

    int readAllMessages = 0;

    int matchedMessages = 0;

    while (!readAllMessages) {
        char *message = NULL;
        size_t charactersRead = 0;

        int x = getline(&message, &charactersRead, fp);

        if (x == -1) {
            readAllMessages = 1;
        } else {
            int matches = messageMatches(ruleTable, message);
        }
    }

    printf("Got all rules\n");
    for (int i; i < 5; i++) {
        printf("Rule %d: %s\n", i, ht_get(ruleTable, i));
    }
}