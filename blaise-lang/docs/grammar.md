# BLAISE Grammar

```
ENTRY_POINT := PROGRAM | MODULE

PROGRAM := 'program' IDENTIFIER (SUBROUTINE_PARAMETERS)? ';' SUBROUTINE* VARIABLES_CLAUSES? '.' COMPOUND_STATEMENT
MODULE := # WORK IN PROGRESS

IDENTIFIER := /[a-zA-z][a-zA-z0-9_]*/
NUMBER := /[0-9]+(\\.[0-9]+(f|d)?)?/ | 0b[0-1]+ | 0o[0-7]+ | 0x[0-9A-Fa-f]+
STRING_LITERAL := /"([^"]|\\.|\\u[0-9a-fA-F]{4})*"/
CHAR_LITERAL := /'([^']|\\.|\\u[0-9a-fA-F]{4})'/
BOOLEAN_LITERAL := 'true' | 'false'

VARIABLES_CLAUSES := 'var' VARIABLE_CLAUSE*
VARIABLE_CLAUSE :=  VARIABLE_NAME_LIST ':' VARIABLE_TYPE ';'
VARIABLE_NAME_LIST := IDENTIFIER (',' IDENTIFIER)*
VARIABLE_TYPE := ('unsigned'? 'byte')|('unsigned'? 'integer')|('unsigned'? 'long')|
                 'char'|'string'|
                 'float'|'double'|
                 'boolean' | ARRAY_DEFINITION
ARRAY_DEFINITION := 'array' '<' VARIABLE_TYPE '>' '[' INTEGER_LITERAL ']'

COMPOUND_STATEMENT := 'begin' STATEMENT* 'end'
STATEMENT := SIMPLE_STATEMENT | COMPLEX_STATEMENT 
SIMPLE_STATEMENT := (FUNCTION_CALL | ASSIGNMENT) ';'
COMPLEX_STATEMENT = COMPOUND_STATEMENT | IF_STATEMENT | FOR_STATEMENT | DOWHILE_STATEMENT | WHILE_STATEMENT | REPET_UNTIL_STATEMENT

FUNCTION_CALL := IDENTIFIER '(' FUNCTION_CALL_PARAMETERS? ')'
FUNCTION_CALL_PARAMETERS := EXPRESSION (',' EXPRESSION)*

ASSIGNMENT := (IDENTIFIER ('['  EXPRESSION ']')? ) ':=' EXPRESSION
IF_STATEMENT := 'if' EXPRESSION 'then' STATEMENT ('else' STATEMENT)?

FOR_STATEMENT := 'for' IDENTIFIER 'from' EXPRESSION 'to' EXPRESSION ('step' EXPRESSION)? COMPOUND_STATEMENT
DOWHILE_STATEMENT := 'do' COMPOUND_STATEMENT 'while' EXPRESSION ';'
REPEAT_UNTIL_STATEMENT := 'repeat' COMPOUND_STATEMENT 'until' EXPRESSION ';'
WHILE_STATEMENT := 'while' EXPRESSION COMPOUND_STATEMENT

# Expression idea from https://en.wikipedia.org/wiki/Operator-precedence_parser
# but I added a few more levels and operator
EXPRESSION := OR_EXPRESSION
OR_EXPRESSION := AND_EXPRESSION ( 'eager'? 'or' AND_EXPRESSION)*
AND_EXPRESSION := EQUALITY_EXPRESSION ( 'eager'? 'and' EQUALITY_EXPRESSION)*
EQUALITY_EXPRESSION := INEQUALITY_EXPRESSION (('==' | '<>' ) INEQUALITY_EXPRESSION)*
INEQUALITY_EXPRESSION := ADDITIVE_EXPRESSION (('<' | '<=' | '>=' | '>') ADDITIVE_EXPRESSION )*
ADDITIVE_EXPRESSION := MULTIPLICATIVE_EXPRESSION (('+' | '-' ) MULTIPLICATIVE_EXPRESSION )*
MULTIPLICATIVE_EXPRESSION := EXPRESSION_TERM (('*' | '/' | '%' ) EXPRESSION_TERM)*
EXPRESSION_TERM := '(' EXPRESSION ')' | NUMBER | IDENTIFIER('[' EXPRESSION ']') | FUNCTION_CALL | '-' EXPRESSION | 'not' EXPRESSION | STRING_LITERAL | CHAR_LITERAL | BOOLEAN_LITERAL | CAST_EXPRESSION | IF_EXPRESSION
CAST_EXPRESSION := 'cast' '<' VARIABLE_TYPE '>' '(' EXPRESSION ')'
IF_EXPRESSION := 'if' EXPRESSION 'then' EXPRESSION 'else' EXPRESSION

# Sub-routines
SUBROUTINE := (FUNCTION_SUBROUTINE_SIGNATURE | PROCEDURE_SUBROUTINE_SIGNATURE) VARIABLES_CLAUSES? COMPOUND_STATEMENT? ';'
FUNCTION_SUBROUTINE_SIGNATURE := 'function' ('native')? IDENTIFIER '(' SUBROUTINE_PARAMETERS? ')' ':' VARIABLE_TYPE
PROCEDURE_SUBROUTINE_SIGNATURE := 'procedure' ('native')? IDENTIFIER '(' SUBROUTINE_PARAMETERS? ')'
SUBROUTINE_PARAMETERS := SUBROUTINE_PARAMETER (',' SUBROUTINE_PARAMETER)*
SUBROUTINE_PARAMETER := VARIABLE_NAME_LIST ':' VARIABLE_TYPE
```
# Automatic type conversion
| From / To | integer | float | double | char | string | boolean |
| --- | --- | --- | --- | --- | --- | --- |
| integer | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |  |
| float |  | :white_check_mark: | :white_check_mark: |  |  |  |
| double |  |  | :white_check_mark: |  |  |  |
| char |  |  |  | :white_check_mark: |  |  |
| string |  |  |  |  | :white_check_mark: |  |
| boolean |  |  |  |  |  | :white_check_mark: |

# Forced type converstion
| From / To | integer | float | double | char | string | boolean |
| --- | --- | --- | --- | --- | --- | --- |
| integer | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  | :white_check_mark: |
| float | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |  |
| double | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |  |
| char | :white_check_mark: |  |  |  | :white_check_mark: |  |
| string |  |  |  |  | :white_check_mark: |  |
| boolean | :white_check_mark: |  |  |  |  | :white_check_mark: |
