%{

enum Opcode
{
	OPCODE_INIT,
	OPCODE_WORD,
	OPCODE_EXECUTE,
	OPCODE_REDIRECT,
	OPCODE_DUPLICATE,
	OPCODE_IN,
	OPCODE_OUT,
	OPCODE_STDIN,
	OPCODE_STDOUT,
	OPCODE_STDERR,
	OPCODE_FILE_DESCRIPTOR,
	OPCODE_PIPE_IO,
	opCODE_PIPE_ERR,
	OPCODE_FOREGROUND,
	OPCODE_IF,
	OPCODE_ELIF,
	OPCODE_ELSE,
	OPCODE_TEST,
	OPCODE_CASE_SWITCH,
	OPCODE_WHILE,
	OPCODE_UNLESS,
	OPCODE_FOR,
	OPCODE_SEQUENCE,
	OPCODE_SUBSTITUTE,
	OPCODE_LOAD_VAR,
	OPCODE_STORE_VAR,
	OPCODE_LOAD_FUNCTION,
	OPCODE_STORE_FUNCTION,
	OPCODE_LOAD_PARAMETER,
	OPCODE_STORE_PARAMETER,
	OPCODE_PUSH_STR,
	OPCODE_PUSH_PREFIX,
	OPCODE_PUSH_ARGUMENTS,
	OPCODE_TEST_PATTERN,
	OPCODE_TEST_EXPR,
	OPCODE_TERMINATE_HERE,
	OPCODE_FILE_PATH,
	OPCODE_SUBSHELL,
	OPCODE_END,
};

void execute_instruction(struct Stack* stack, enum Opcode opcode)
{
 switch (opcode) {
    case OPCODE_INIT:
        
        break;
    case OPCODE_WORD:
        
        break;
    case OPCODE_EXECUTE:
        
        break;
    case OPCODE_REDIRECT:
        
        break;
    case OPCODE_DUPLICATE:
        
        break;
    case OPCODE_IN:
        
        break;
    case OPCODE_OUT:
        
        break;
    case OPCODE_STDIN:
        
        break;
    case OPCODE_STDOUT:
        
        break;
    case OPCODE_STDERR:
        
        break;
    case OPCODE_FILE_DESCRIPTOR:
        
        break;
    case OPCODE_PIPE_IO:
        
        break;
    case OPCODE_PIPE_ERR:
        
        break;
    case OPCODE_FOREGROUND:
        
        break;
    case OPCODE_IF:
        
        break;
    case OPCODE_ELIF:
        
        break;
    case OPCODE_ELSE:
        
        break;
    case OPCODE_TEST:
        
        break;
    case OPCODE_CASE_SWITCH:
        
        break;
    case OPCODE_WHILE:
        
        break;
    case OPCODE_UNLESS:
        
        break;
    case OPCODE_FOR:
        
        break;
    case OPCODE_SEQUENCE:
        
        break;
    case OPCODE_SUBSTITUTE:
        
        break;
    case OPCODE_LOAD_VAR:
        
        break;
    case OPCODE_STORE_VAR:
        
        break;
    case OPCODE_LOAD_FUNCTION:
        
        break;
    case OPCODE_STORE_FUNCTION:
        
        break;
    case OPCODE_LOAD_PARAMETER:
        
        break;
    case OPCODE_STORE_PARAMETER:
        
        break;
    case OPCODE_PUSH_STR:
        
        break;
    case OPCODE_PUSH_PREFIX:
        
        break;
    case OPCODE_PUSH_ARGUMENTS:
        
        break;
    case OPCODE_TEST_PATTERN:
        
        break;
    case OPCODE_TEST_EXPR:
        
        break;
    case OPCODE_TERMINATE_HERE:
        
        break;
    case OPCODE_FILE_PATH:
        
        break;
    case OPCODE_SUBSHELL:
        
        break;
    case OPCODE_END:
        return;  
    default:
        fprintf(stderr, "Unknown opcode: %d\n", opcode);
        exit(EXIT_FAILURE);
}

}


%}


%token NAME STRING NUMBER WORD

%%

program : 
        | program command '.' {  }

command : simple_command
        | pipeline
        | if_statement
        | while_loop
        | for_loop
        | case_statement
        | function_definition
        | variable_assignment
        | redirection
        | compound_command
        | test_command
        {  }

simple_command : 
              | simple_command word_list redirections
              {  }

word_list : 
          | word_list word
          {  }

word : STRING
     | variable
     | command_substitution
     | parameter_expansion
     {  }

variable : '$' ( NAME | '{' NAME '}' )
         {  }

command_substitution : "$(" compound_command ")"
                   {  }

parameter_expansion : "${" NAME [ ':' word ] "}"
                   {  }

redirections : 
            | redirections redirection
            {  }

redirection : '<' word
            | '>' word
            | ">>" word
            | "2>" word
            | "2>>" word
            | "&>" word
            | "<<" delimiter
            | "<<<" word
            {  }

delimiter : STRING
          {  }

pipeline : command '|' command 
	 | command
         {  }

if_statement : "if" compound_command "then" compound_command "else" compound_command "fi"
            | "if" compound_command "then" compound_command "fi"
            {  }

while_loop : "while" compound_command "do" compound_command "done"
           {  }

for_loop : "for" NAME "in" word_list "do" compound_command "done"
         {  }

word_list : 
          | word_list word
          {  }

case_statement : "case" word "in" '(' pattern_list ')' compound_command ";;" "esac"
              {  }

pattern_list : word '|' word
	     | word
             {  }

function_definition : "function" NAME "()" compound_command
                   {  }

variable_assignment : NAME '=' word
                   {  }

compound_command : '{' command_list '}'
                {  }

test_command : '[' test_expression ']'
            {  }

test_expression : test_term { test_operator test_term }
               {  }

test_term : unary_test
          | '[' test_expression ']'
          | '!' test_expression
          | test_expression "-a" test_expression
          | test_expression "-o" test_expression
          | '(' test_expression ')'
          {  }

unary_test : unary_operator word
          | unary_operator '[' word ']'
          | unary_operator test_expression
          {  }

unary_operator : "-b" | "-c" | "-d" | "-e" | "-f" | "-g" | "-h" | "-k" | "-p" | "-r" | "-s" | "-t" | "-u" | "-w" | "-x" | "-z"
              {  }

test_operator : "=" | "!=" | "-eq" | "-ne" | "-lt" | "-le" | "-gt" | "-ge"
              {  }

