#pragma once

#include <ostream>
#include <map>
#include <string>

#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise
{
enum class blaise_token : unsigned long
{
   PREPROCESSOR_IF,
   PREPROCESSOR_ELSE,
   PREPROCESSOR_END,
   PREPROCESSOR_DEFINE,
   PREPROCESSOR_DEFINED,
   PREPROCESSOR_UNDEFINE,
   PREPROCESSOR_VERSION,

   // KEYWORDS
   PROGRAM,
   MODULE,
   USES,
   BEGIN,
   END,
   VAR,
   CONST,
   FUNCTION,
   PROCEDURE,
   IN,
   OUT,
   INOUT,
   RETURN,
   IF,
   THEN,
   ELSE,
   ELIF,
   WHILE,
   DO,
   UNTIL,
   SWITCH,
   CASE,
   FOR,
   FOREACH,
   CONTINUE,
   BREAK,
   AND,
   OR,
   NOT,
   XOR,
   SHIFT,
   LEFT,
   RIGHT,
   EAGER,
   LAZY,
   STATIC_CAST,
   TYPE,
   INTERFACE,
   ABSTRACT,
   CONSTRUCTOR,
   DESTRUCTOR,
   PUBLIC,
   PRIVATE,
   PROTECTED,
   STATIC,
   VIRTUAL,
   OVERRIDE,
   INHERIT,
   IMPLEMENT,
   IMPLEMENTATION,
   LAMBDA,
   TRY,
   CATCH,
   FINALLY,
   THROW,
   UNION,
   ENUM,
   NAMEOF,
   TYPEOF,
   ADDRESSOF,
   TYPE_TUPLE,
   TYPE_SET,
   TYPE_DICTIONARY,
   ALIAS,
   NATIVE,
   GENERIC,
   REQUIRE,
   ENSURE,
   ASSERT,
   ASSEMBLY,
   ASYNC,
   AWAIT,

   // TYPES
   TYPE_BYTE,
   TYPE_INTEGER,
   TYPE_LONG,
   UNSIGNED,
   TYPE_CHAR,
   TYPE_STRING,
   TYPE_FLOAT,
   TYPE_DOUBLE,
   TYPE_BOOLEAN,
   TYPE_COMPLEX,
   TYPE_ARRAY,
   OF,
   CONST_TRUE,
   CONST_FALSE,
   POINTER,
   NEW,
   DELETE,
   NULLPTR,

   // PUNCTUATION
   SEMICOLON,
   PERIOD,
   COLON,
   LEFT_BRAKET,
   RIGHT_BRAKET,
   LEFT_SQUARE_BRAKET,
   RIGHT_SQUARE_BRAKET,
   LEFT_ANGLE_BRAKET,
   RIGHT_ANGLE_BRAKET,
   PLUS,
   MINUS,
   MULTIPLY,
   DIVIDE,
   REMINDER,
   ASSIGN,
   EQUAL,
   NOT_EQUAL,
   SPACESHIP,
   RANGE,

   // OTHERS
   ATTRIBUTE,
   IDENTIFIER,
   STRING_CONSTANT,
   INTEGER_CONSTANT,
   CHAR_CONSTANT,
   FLOATING_POINT_CONSTANT,
   COMMENT,
   LINE_COMMENT,
};

class blaise_token_provider
{
   class blaise_token_provider_constructor : public gasp::common::token_provider_constructor<blaise_token>
   {
   public:
      blaise_token_provider_constructor();
   };

   static blaise_token_provider_constructor _private;

public:
   static typename std::vector<blaise_token>::const_iterator cbegin();
   static typename std::vector<blaise_token>::const_iterator cend();

   static std::string rule(blaise_token token);
   static std::string name(blaise_token token);
   static bool keep_value(blaise_token token);
   static bool keep_token(blaise_token token);
};

std::ostream &operator<<(std::ostream &os, const blaise_token &tok);

} // namespace gasp::blaise