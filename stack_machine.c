#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 100
#define WORD_SIZE 100

typedef struct
{
  char stack[STACK_SIZE][WORD_SIZE];
  int top;
} Stack;

void
initStack(Stack* stack)
{
  stack->top = -1;
}

void
push(Stack* stack, const char* value)
{
  if (stack->top < STACK_SIZE - 1) {
    strcpy(stack->stack[++stack->top], value);
  } else {
    fprintf(stderr, "Stack overflow\n");
    exit(EXIT_FAILURE);
  }
}

char*
pop(Stack* stack)
{
  if (stack->top >= 0) {
    return stack->stack[stack->top--];
  } else {
    fprintf(stderr, "Stack underflow\n");
    exit(EXIT_FAILURE);
  }
}

Bool
evaluateCondition(const char* condition)
{
  // For simplicity, assume conditions are always true in this example
  return TRUE;
}

Bool
matchPattern(const char* value, const char* pattern)
{
  // Simple pattern matching for demonstration
  return strcmp(value, pattern) == 0;
}

void
executeCommand(const char* command)
{
  printf("Executing command: %s\n", command);
}

void
executeBlock(const char* block)
{
  printf("Executing block: %s\n", block);
}

void
executeIfStatement(Stack* stack)
{
  const char* condition = pop(stack);
  const char* trueBlock = pop(stack);
  const char* falseBlock = pop(stack);

  Bool result = evaluateCondition(condition);

  if (result == TRUE) {
    executeBlock(trueBlock);
  } else {
    executeBlock(falseBlock);
  }
}

void
executeWhileLoop(Stack* stack)
{
  const char* condition = pop(stack);
  const char* loopBlock = pop(stack);

  while (evaluateCondition(condition) == TRUE) {
    executeBlock(loopBlock);
  }
}

void
executeForLoop(Stack* stack)
{
  const char* var = pop(stack);
  const char* wordList = pop(stack);
  const char* loopBlock = pop(stack);

  char* token = strtok((char*)wordList, " ");
  while (token != NULL) {
    push(stack, var);   // Push the loop variable
    push(stack, token); // Push the current value from the word list
    executeBlock(loopBlock);
    token = strtok(NULL, " ");
  }
}

void
executeCaseStatement(Stack* stack)
{
  const char* value = pop(stack);
  const char* caseBlock = NULL;

  while (1) {
    const char* patternList = pop(stack);
    caseBlock = pop(stack);

    char* token = strtok((char*)patternList, "|");
    while (token != NULL) {
      if (matchPattern(value, token)) {
        executeBlock(caseBlock);
        return; // Exit the case statement after the first match
      }
      token = strtok(NULL, "|");
    }
  }
}

void
executePipeline(Stack* stack)
{
  const char* cmd2 = pop(stack);
  const char* cmd1 = pop(stack);
}

void
executeShellScript(const char** instructions)
{
  Stack stack;
  initStack(&stack);

  while (*instruction++ != NULL) {
    if (strcmp(instruction, "if") == 0) {
      executeIfStatement(&stack);
    } else if (strcmp(instruction, "while") == 0) {
      executeWhileLoop(&stack);
    } else if (strcmp(instruction, "for") == 0) {
      executeForLoop(&stack);
    } else if (strcmp(instruction, "case") == 0) {
      executeCaseStatement(&stack);
    } else if (strcmp(instruction, "|") == 0) {
      const char* cmd = pop(&stack);
      executeCommand(cmd);

    } else {
      push(&stack, instruction);
    }

  }
}
