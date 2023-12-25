program: command_list
        | command_list NEWLINE
        ;

command_list: command
            | command_list separator command
            ;

separator: SEMICOLON
        | AND
        | OR
        | PIPE
        ;

command: simple_command
       | pipeline
       | if_statement
       | while_loop
       | for_loop
       | case_statement
       | function_definition
       | variable_assignment
       | comment
       | redirection
       | compound_command
       | test_command
       ;

simple_command: word_list
             | word_list redirection
             ;

redirection: REDIRECT WORD
           ;

compound_command: '{' command_list '}'
               ;

test_command: '[' test_expression ']'
            ;

test_expression: test_term
              | test_term test_operator test_term
              ;

test_term: unary_test
         | '[' test_expression ']'
         | '!' test_expression
         | test_expression "-a" test_expression
         | test_expression "-o" test_expression
         | '(' test_expression ')'
         ;

unary_test: unary_operator WORD
          | unary_operator '[' WORD ']'
          | unary_operator test_expression
          ;

unary_operator: "-b" | "-c" | "-d" | "-e" | "-f" | "-g" | "-h" | "-k" | "-p" | "-r" | "-s" | "-t" | "-u" | "-w" | "-x" | "-z"
            ;

test_operator: '=' | "!=" | "-eq" | "-ne" | "-lt" | "-le" | "-gt" | "-ge"
            ;

if_statement: "if" compound_command "then" compound_command "else" compound_command "fi"
            | "if" compound_command "then" compound_command "fi"
            ;

while_loop: "while" compound_command "do" compound_command "done"
          ;

for_loop: "for" name "in" word_list "do" compound_command "done"
        ;

word_list: WORD
	 | word_list WORD
         ;

case_statement: "case" word "in" '(' pattern_list ')' compound_command "esac"
             ;

pattern_list: WORD
	    | pattern_list '|' WORD
            ;

function_definition: "function" name "()" compound_command
                  ;

variable_assignment: name '=' word
                  ;

comment: "#" { any_character }
       ;

compound_command: '{' { command } '}'
               ;

parameter_expansion: "${" name [ ":" word ] '}'
                  ;

command_substitution: "$(" compound_command ')'
                   ;

variable: '$' name 
	|  "${" name '}'
        ;

string: { any_character }
      ;

