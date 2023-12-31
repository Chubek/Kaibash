#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdarg.h>
#include <regex.h>
#include <assert.h>

#include "machine.h"


extern struct Stack* stack;

void execute_jump(struct Stack* stack) {
    struct StackValue* offset = pop(stack);

    if (if_value_opcode(offset))
}


void execute_instruction(Stack* stack, Opcode opcode) {
    switch (opcode) {
        case OPCODE_TEST_TERM_NORMAL:
            execute_test_term_normal(stack);
            break;
        case OPCODE_TEST_TERM_NEGATE:
            execute_test_term_negate(stack);
            break;
        
        default:
            
            break;
    }
}


