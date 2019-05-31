#pragma once

namespace gasp::blaise::language {

enum class blaise_language_expression_type {
   UNDEFINED,
   BINARY,
   UNARY,
   VARIABLE,
   FUNCTION_CALL,
   LITERAL_INTEGER,
   LITERAL_FLOAT,
   LITERAL_DOUBLE,
   LITERAL_CHAR,
   LITERAL_STRING,
   LITERAL_BOOLEAN,
};

}