#pragma once

namespace gasp::blaise::ast {

enum class blaise_ast_expression_type {
   UNDEFINED,
   BINARY,
   UNARY,
   MEMORY_LOCATION,
   FUNCTION_CALL,
   LITERAL_INTEGER,
   LITERAL_FLOAT,
   LITERAL_DOUBLE,
   LITERAL_CHAR,
   LITERAL_STRING,
   LITERAL_BOOLEAN,
   CAST,
   TERNARY
};

}