#pragma once

namespace gasp::blaise
{

enum class blaise_token_type : unsigned char
{
   UNKNOWN = 0,

   // KEYWORDS
   PROGRAM = 1,
   MODULE,
   USE,
   BEGIN,
   END,
   VAR,
   LOGICAL_AND,
   LOGICAL_OR,
   LOGICAL_NOT,
   LOGICAL_EAGER_AND,
   LOGICAL_EAGER_OR,
   FUNCTION,
   PROCEDURE,
   NATIVE,
   CAST,
   IF,
   THEN,
   ELSE,
   DO,
   WHILE,
   REPEAT,
   UNTIL,
   FOR,
   FROM,
   TO,
   STEP,
   NEW,
   DELETE,

   // TYPES
   TYPE_INTEGER,
   TYPE_CHAR,
   TYPE_STRING,
   TYPE_FLOAT,
   TYPE_DOUBLE,
   TYPE_BOOLEAN,
   TYPE_ARRAY,
   TYPE_LONG,
   TYPE_SHORT,
   TYPE_BYTE,

   // PUNCTUATION
   ASSIGNMENT,
   SEMICOLON,
   PERIOD,
   COLON,
   COMMA,
   LEFT_PARENTHESES, RIGHT_PARENTHESES,
   LEFT_BRACKET, RIGHT_BRACKET,
   GREAT_THAN, LESS_THAN, GREAT_THAN_OR_EQUAL_TO, LESS_THAN_OR_EQUAL_TO,
   EQUAL_TO, NOT_EQUAL_TO,
   PLUS, MINUS,
   MULTIPLY, DIVIDE, REMAINDER,
  
   // OTHERS
   IDENTIFIER,
   INTEGER_LITERAL,
   FLOAT_LITERAL,
   DOUBLE_LITERAL,
   BYTE_LITERAL,
   SHORT_LITERAL,
   LONG_LITERAL,
   INTEGER_BASE_TWO_LITERAL,
   INTEGER_BASE_EIGHT_LITERAL,
   INTEGER_BASE_SIXTEEN_LITERAL,
   STRING_LITERAL,
   CHAR_LITERAL,
   BOOLEAN_LITERAL,
};
}