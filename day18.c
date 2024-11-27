#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int N;

// Adapted stack implementation from https://www.log2base2.com/data-structures/stack/stack-using-linked-list.html
typedef struct node
{
    long long data;
    struct node *next;
} node_t;

void push(node_t **head, long long val)
{
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->data = val;
    newNode->next = *head;
    *head = newNode;
}

long long pop(node_t **head)
{
    if (*head == NULL)
    {
        return -1;
    }

    long long val = (*head)->data;
    node_t *temp = *head;
    *head = (*head)->next;
    free(temp);
    return val;
}

void printList(node_t **head)
{
    node_t *temp = *head;

    while (temp != NULL)
    {
        if (temp->data == '(' || temp->data == ')' || temp->data == '+' || temp->data == '*')
        {
            printf("%c->", (char)(temp->data));
        }
        else
        {
            printf("%lld->", temp->data);
        }
        temp = temp->next;
    }
    printf("NULL\n");
}

// implementation of dijsktra's shunting yard algorithm using pseudocode from
// https://www.geeksforgeeks.org/expression-evaluation/
long long evaluateLine(FILE *fp)
{
    node_t *operatorStack = NULL;
    node_t *valueStack = NULL;

    char *line = NULL;
    size_t charactersRead = 0;

    getline(&line, &charactersRead, fp);

    // 1. While there are still tokens to be read in,
    for (int i = 0; i < charactersRead; i++)
    {
        // 1.1 Get the next token.
        char nextToken = line[i];

        // 1.2 If the token is:
        if (nextToken == ' ')
        {
            continue;
        }
        else if (nextToken <= 57 && nextToken >= 48)
        // 1.2.1 A number: push it onto the value stack.
        {
            long long value = nextToken - '0';
            push(&valueStack, value);
        }
        else if (nextToken == '(')
        // 1.2.3 A left parenthesis: push it onto the operator stack.
        {
            push(&operatorStack, nextToken);
        }
        else if (nextToken == ')')
        // 1.2.4 A right parenthesis:
        {
            // 1 While the thing on top of the operator stack is not a
            //   left parenthesis,
            while (operatorStack->data != '(')
            {
                // 1 Pop the operator from the operator stack.
                char operator= pop(&operatorStack);
                // 2 Pop the value stack twice, getting two operands.
                long long operand2 = pop(&valueStack);
                long long operand1 = pop(&valueStack);
                // 3 Apply the operator to the operands, in the correct order.
                long long result;
                if (operator== '+')
                {
                    result = operand1 + operand2;
                }
                else if (operator== '*')
                {
                    result = operand1 * operand2;
                }
                // 4 Push the result onto the value stack.
                push(&valueStack, result);
            }
            pop(&operatorStack);
        }
        else if (nextToken == '+' || nextToken == '*')
        // 1.2.5 An operator (call it thisOp):
        {
            // 1 While the operator stack is not empty
            while (operatorStack != NULL && (operatorStack->data == '+' || operatorStack->data == '*'))
            {
                // 1 Pop the operator from the operator stack.
                char operator= pop(&operatorStack);
                long long operand2 = pop(&valueStack);
                long long operand1 = pop(&valueStack);
                long long result;
                if (operator == '+')
                {
                    result = operand1 + operand2;
                }
                else if (operator == '*')
                {
                    result = operand1 * operand2;
                }
                push(&valueStack, result);
            }

            // 2 Push thisOp onto the operator stack.
            push(&operatorStack, nextToken);
        }
    }
    // 2. While the operator stack is not empty,
    while (operatorStack != NULL)
    {
        // 1 Pop the operator from the operator stack.
        char operator = pop(&operatorStack);
        // 2 Pop the value stack twice, getting two operands.
        long long operand2 = pop(&valueStack);
        long long operand1 = pop(&valueStack);
        // 3 Apply the operator to the operands, in the correct order.
        long long result;
        if (operator == '+')
        {
            result = operand1 + operand2;
        }
        else if (operator == '*')
        {
            result = operand1 * operand2;
        }

        // 4 Push the result onto the value stack.
        push(&valueStack, result);
    }
    free(line);

    long long val = pop(&valueStack);
    return val;
}

int main(int argc, char **argv)
{
    FILE *fp;
    N = atoi(argv[2]);

	if (!(fp = fopen(argv[1],"r"))) {
		return 255;
	}

    long long sum = 0;
    for (long long i = 0; i < N; i++) {
        sum = sum + evaluateLine(fp);
    }

    printf("Answer = %lld\n", sum);
}