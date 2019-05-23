# BLAISE Grammar

```javascript
ENTRY_POINT -> PROGRAM | MODULE

PROGRAM -> 'program' IDENTIFIER ';' USES_CLAUSES? VARIABLES_CLAUSES? 'begin' STATEMENT* 'end' '.'
IDENTIFIER -> /[a-zA-z][a-zA-z0-9_]*/

MODULE -> /* WORK IN PROGRESS */

```

