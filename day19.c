#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stringtable.h"
#include <regex.h>

int N;
int messageMatchesRuleNumber(hashtable *ruleTable, char **message, int ruleNumber);

regex_t single_char_regex;
regex_t multi_rule_regex;
regex_t or_regex;

int messageMatchesRule(hashtable *ruleTable, char **message, char *ruleString) {
    char *ruleNo;
    char *saveptr; 

    printf("One side of the rule: %s\n", ruleString);

    // Get the first token
    ruleNo = strtok_r(ruleString, " ", &saveptr);
    if (!messageMatchesRuleNumber(ruleTable, message, atoi(ruleNo))) {
        return 0;
    }

    while (ruleNo != NULL) {
        // Get the next token
        ruleNo = strtok_r(NULL, " ", &saveptr);
        if (!messageMatchesRuleNumber(ruleTable, message, atoi(ruleNo))) {
            return 0;
        }
    }
    return 1;
}

int messageMatchesRuleNumber(hashtable *ruleTable, char **messagePtr, int rule) {
    char *ruleString = ht_get(ruleTable, rule);
    printf("Rule: %s\n", ruleString);

    if (regexec(&single_char_regex, ruleString, 0, NULL, 0) == 0) {
        if ((*messagePtr)[0] == ruleString[1]) {
            printf("Matching single character %s against current message %s\n", ruleString, *messagePtr);
            printf("after this, message will be %s\n", ++(*messagePtr));
            // if the message's first character matches the rule's character,
            // change the message pointer to the next character
            return 1;
        } else {
            return 0;
        }
    }


    if (regexec(&or_regex, ruleString, 0, NULL, 0) == 0) {
        printf("Matching or regex on message %s\n", *messagePtr);
        char *rule;
        char *saveptr; 

        // Get the left rule
        rule = strtok_r(ruleString, "|", &saveptr);
        if (messageMatchesRule(ruleTable, messagePtr, rule)) {
            return 1;
        } else {
            rule = strtok_r(NULL, "|", &saveptr);
            return messageMatchesRule(ruleTable, messagePtr, rule);
        }
    }

    if (regexec(&multi_rule_regex, ruleString, 0, NULL, 0) == 0) {
        printf("Matching multi rule regex on message %s\n", *messagePtr);
        char *ruleNo;
        char *saveptr; 

        // Get the first token
        ruleNo = strtok_r(ruleString, " ", &saveptr);
        if (!messageMatchesRuleNumber(ruleTable, messagePtr, atoi(ruleNo))) {
            return 0;
        }

        while (ruleNo != NULL) {
            // Get the next token
            ruleNo = strtok_r(NULL, " ", &saveptr);
            if (!messageMatchesRuleNumber(ruleTable, messagePtr, atoi(ruleNo))) {
                return 0;
            }
        }
        return 1;
    }

    printf("didn't match any regexes");
    return 1;
}

int main(int argc, char **argv)
{
    regcomp(&single_char_regex, "\"[ab]\"", REG_EXTENDED);
    regcomp(&multi_rule_regex, "([0-9]*|\\s)*", REG_EXTENDED);
    regcomp(&or_regex, "([0-9]*|\\s)*\\|(\\s|[0-9]*)*", REG_EXTENDED);

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
            rule++;

            ht_put(ruleTable, atoi(ruleNo), rule);
        }

    }

    for (int i = 0; i <= 5; i++) {
        printf("Rule %d: %s\n", i, ht_get(ruleTable, i));
    }

    int readAllMessages = 0;

    int matchedMessages = 0;

    while (!readAllMessages) {
        char *message = NULL;
        size_t charactersRead = 0;

        int x = getline(&message, &charactersRead, fp);

        char **messagePtr = &message;

        if (x == -1) {
            readAllMessages = 1;
        } else {
            printf("Message: %s\n", *messagePtr);
            int matches = messageMatchesRuleNumber(ruleTable, messagePtr, 0);
            printf("Matches? %d\n", matches);
            matchedMessages += matches;
        }
    }

    printf("Matched messages: %d\n", matchedMessages);

}