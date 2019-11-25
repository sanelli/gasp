# BLAISE Grammar

```EBNF
(* Main entry points definition *)
ENTRY_POINT = PROGRAM | MODULE ;
PROGRAM = "program" IDENTIFIER [ SUBROUTINE_PARAMETERS ] [":" VARIABLE_TYPE] ";" {SUBROUTINE} {CONST_CLAUSES} {VARIABLES_CLAUSES} COMPOUND_STATEMENT  "." ;
MODULE = 'module' {SUBROUTINE} "end" ".";

(* Commons *)
LETTER = "A" | "B" | "C" | "D" | "E" | "F" | "G"
       | "H" | "I" | "J" | "K" | "L" | "M" | "N"
       | "O" | "P" | "Q" | "R" | "S" | "T" | "U"
       | "V" | "W" | "X" | "Y" | "Z" | "a" | "b"
       | "c" | "d" | "e" | "f" | "g" | "h" | "i"
       | "j" | "k" | "l" | "m" | "n" | "o" | "p"
       | "q" | "r" | "s" | "t" | "u" | "v" | "w"
       | "x" | "y" | "z" ;
DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
UNSERSCORE = "_" ;
ALL_CHARACTERS = ? all visible characters ? ;

BINARY_DIGIT = "0" | "1" ;
OCTAL_DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" ;
HEX_DIGIT = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" | "A" | "B" | "C" | "D" | "E" | "F" | "a" | "b" | "c" | "d" | "e" | "f" ;

(* Identifier *)
IDENTIFIER = LETTER { LETTER | DIGIT | UNDERSCORE } ;

(* Numbers *)
INTEGER_LITERAL = DECIMAL_INTEGER_LITERAL
      | BINARY_INTEGER_LITERAL
      | OCTAL_INTEGER_LITERAL
      | HEXADECIMAL_INTEGER_LITERAL
 ;
DECIMAL_INTEGER_LITERAL = DIGIT {DIGIT} ;
BINARY_INTEGER_LITERAL = "0b" BINARY_DIGIT {BINARY_DIGIT};
OCTAL_INTEGER_LITERAL = '0o' OCTAL_DIGIT {OCTAL_DIGIT} ;
HEXADECIMAL_INTEGER_LITERAL = '0x' HEX_DIGIT {HEX_DIGIT} ;
NUMBER = BYTE_LITERAL
      | SHORT_LITERAL 
      | INTEGER_LITERAL
      | LONG_LITERAL
      | FLOAT_LITERAL
      | DOUBLE_LITERAL ;
BYTE_LITERAL = DECIMAL_INTEGER_LITERAL ("b" | "B") ;
SHORT_LITERAL = DECIMAL_INTEGER_LITERAL ("s" | "S") ;
LONG_LITERAL = DECIMAL_INTEGER_LITERAL ("l" | "L") ;
FLOAT_LITERAL = DECIMAL_INTEGER_LITERAL ["." DECIMAL_INTEGER_LITERAL ] ("f" | "F") ;
DOUBLE_LITERAL = DECIMAL_INTEGER_LITERAL ["." DECIMAL_INTEGER_LITERAL ] ["d" | "D"] ;

(* Other literals *)
STRING_ESCAPE_SEQUENCE = "\\" ('"' | "n" | "f" |"t" | "a" | "r" ) ;
STRING_LITERAL = '"' { (ALL_CHARACTERS - '"') | (STRING_ESCAPE_SEQUENCE) } '"' ;
CHAR_ESCAPE_SEQUENCE = "\\" ("'" | "n" | "f" |"t" | "a" | "r" ) ;
CHAR_LITERAL = '"' ((ALL_CHARACTERS - '"') | (CHAR_ESCAPE_SEQUENCE)) '"' ;
BOOLEAN_LITERAL = "true" | "false" ;

(* Types and type definition  *)
VARIABLES_CLAUSES = 'var' VARIABLE_CLAUSE {VARIABLE_CLAUSE} ;
VARIABLE_CLAUSE =  VARIABLE_NAME_LIST ":" VARIABLE_TYPE ";" ;
VARIABLE_NAME_LIST = IDENTIFIER {',' IDENTIFIER} ;
VARIABLE_TYPE =  VARIABLE_INTEGRAL_TYPE | ARRAY_DEFINITION ;
VARIABLE_BASE_TYPE = "byte" | "short" | "integer"| "long" | "char" | "string" | "float" |"double" | "boolean";
ARRAY_DEFINITION = "array" "<" VARIABLE_TYPE ">" "[" [INTEGER_LITERAL  {',' INTEGER_LITERAL} ] "]" ;
CONST_CLAUSES = 'const' CONST_CLAUSE {CONST_CLAUSE} ;
VARIABLE_CLAUSE =  IDENTIFIER ':=' CONST_LITERAL ';';
CONST_LITERAL = NUMBER | CHAR_LITERAL | BOOLEAN_LITERAL | STRING_LITERAL ;

(* Statement *)
COMPOUND_STATEMENT = "begin" {STATEMENT} "end" ;
STATEMENT = SIMPLE_STATEMENT | COMPLEX_STATEMENT ;
SIMPLE_STATEMENT = {FUNCTION_CALL | ASSIGNMENT | FREE_STATEMENT} ";" ;
COMPLEX_STATEMENT = COMPOUND_STATEMENT | IF_STATEMENT | FOR_STATEMENT | DOWHILE_STATEMENT | WHILE_STATEMENT | REPEAT_UNTIL_STATEMENT ;

FUNCTION_CALL = IDENTIFIER "(" {FUNCTION_CALL_PARAMETERS} ")" ;
FUNCTION_CALL_PARAMETERS = EXPRESSION {"," EXPRESSION} ;

ASSIGNMENT = IDENTIFIER ["[" EXPRESSION "]"] ":=" EXPRESSION ;

FREE_STATEMENT = "delete" "(" IDENTIFIER ")" ;

IF_STATEMENT = 'if' EXPRESSION 'then' STATEMENT ['else' STATEMENT] ;
FOR_STATEMENT = 'for' IDENTIFIER 'from' EXPRESSION 'to' EXPRESSION ['step' EXPRESSION] STATEMENT ;
DOWHILE_STATEMENT = 'do' STATEMENT 'while' EXPRESSION ';' ;
REPEAT_UNTIL_STATEMENT = 'repeat' STATEMENT 'until' EXPRESSION ';' ;
WHILE_STATEMENT = 'while' EXPRESSION STATEMENT ;

(* Expression *)
EXPRESSION = '(' EXPRESSSION ')' 
            | UNARY_OPERATOR EXPRESSION 
            | EXPRESSION BINRARY_OPERATOR EXPRESSION
            | LITERAL_EXPRESSION
            | IF_EXPRESSION
            | CAST_EXPRESSION
            | ALLOCATE_EXPRESSION ;

UNARY_OPERATOR = BOOLEAN_UNARY_OPERATOR | MATH_UNARY_OPERATOR ;
BOOLEAN_UNARY_OPERATOR = 'not' ;
MATH_UNARY_OPERATOR = '-' ;
BINRARY_OPERATOR = BOOLEAN_BINARY_OPERATOR | MATH_BINRARY_OPERATOR | COMPARISON_BINARY_OPERATOR ;
BOOLEAN_BINARY_OPERATOR = 'and' | 'not' ;
MATH_BINARY_OPERATOR = '+' | '-' | '*' | '/' | '%' ;
COMPARISON_BINARY_OPERATOR = '==' | '<>' | '>' | '>=' | '<' | '<=' ;
LITERAL_EXPRESSION = NUMBER | STRING_LITERAL | CHAR_LITERAL | BOOLEAN_LITERAL ;
CAST_EXPRESSION = 'cast' '<' VARIABLE_TYPE '>' '(' EXPRESSION ')' ;
IF_EXPRESSION = 'if' EXPRESSION 'then' EXPRESSION 'else' EXPRESSION ;
ALLOCATE_EXPRESSION = 'new' '<' VARIABLE_TYPE '>' '(' IDENTTIFIER ')' ;

(* Subroutines *)
SUBROUTINE = (FUNCTION_SUBROUTINE_SIGNATURE | PROCEDURE_SUBROUTINE_SIGNATURE) ';' {SUBROUTINE_CLAUSES} {VARIABLES_CLAUSES} [COMPOUND_STATEMENT] ';' ;
FUNCTION_SUBROUTINE_SIGNATURE = ['declare'] 'function' ['native'] IDENTIFIER '(' {SUBROUTINE_PARAMETERS} ')' ':' VARIABLE_TYPE ;
PROCEDURE_SUBROUTINE_SIGNATURE = ['declare'] 'procedure' ['native'] IDENTIFIER '(' {SUBROUTINE_PARAMETERS} ')' ;
SUBROUTINE_PARAMETERS = SUBROUTINE_PARAMETER {',' SUBROUTINE_PARAMETER} ;
SUBROUTINE_PARAMETER = VARIABLE_NAME_LIST ':' VARIABLE_TYPE ;
```
# Automatic type conversion
This table shows the cast that automatically are performed inside `Blaise` parser.

| From / To | byte | short | integer |long | float | double | char | boolean |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **byte** |  | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |
| **short** |  |  | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |
| **integer** |  |  |  | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |
| **long** |  |  |  |  | :white_check_mark: | :white_check_mark: |  |  |
| **float** |  |  |  |  |  | :white_check_mark: |  |  |
| **double** |  |  |  |  |  |  |  |  |
| **char** |  |  |  |  |  |  |  |  |
| **boolean** |  |  |  |  |  |  |  |  |


# Forced type converstion
This table shows the cast that automatically are performed via `cast<>` operator inside expressions in `Blaise`.

| From / To | byte | short | integer |long | float | double | char | boolean |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **byte** | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **short** | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **integer** | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **long** | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |
| **float** | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |
| **double** | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |
| **char** | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |  | :white_check_mark: |  |
| **boolean** | :white_check_mark: | :white_check_mark: | :white_check_mark: |  |  |  |  | :white_check_mark: |
