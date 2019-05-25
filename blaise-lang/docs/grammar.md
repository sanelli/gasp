# BLAISE Grammar

```javascript
ENTRY_POINT => PROGRAM | MODULE

PROGRAM => 'program' IDENTIFIER ';' VARIABLES_CLAUSES? 'begin' STATEMENT* 'end' '.'
IDENTIFIER => /[a-zA-z][a-zA-z0-9_]*/

VARIABLES_CLAUSES => 'var' VARIABLE_CLAUSE*
VARIABLE_CLAUSE =>  VARIABLE_NAME_LIST ':' VARIABLE_TYPE ';'
VARIABLE_NAME_LIST => IDENTIFIER (',' IDENTIFIER)*
VARIABLE_TYPE => ('unsigned'? 'byte')|('unsigned'? 'integer')|('unsigned'? 'long')|
                 'char'|'string'|
                 'float'|'double'|
                 'boolean'

MODULE -> /* WORK IN PROGRESS */

```

