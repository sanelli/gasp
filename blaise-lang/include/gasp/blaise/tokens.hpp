#pragma once

#include <ostream>
#include <string>

#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise
{

enum class blaise_token_type : unsigned char
{
   // KEYWORDS
   PROGRAM,
   MODULE,
   USES,
   BEGIN,
   END,
   VAR,
   LOGICAL_AND,
   LOGICAL_OR,
   LOGICAL_NOT,
   LOGICAL_EAGER,

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

   // PUNCTUATION
   ASSIGNMENT,
   SEMICOLON,
   PERIOD,
   COLON,
   COMMA,
   LEFT_PARENTHESES,
   RIGHT_PARENTHESES,
   GREAT_THAN, LESS_THAN, GREAT_THAN_OR_EQUAL_TO, LESS_THAN_OR_EQUAL_TO,
   EQUAL_TO, NOT_EQUAL_TO,
   PLUS, MINUS,
   MULTIPLY, DIVIDE, REMAINDER,
  
   // OTHERS
   IDENTIFIER,
   INTEGER_LITERAL,
   DECIMAL_LITERAL,
   INTEGER_BASE_TWO_LITERAL,
   INTEGER_BASE_EIGHT_LITERAL,
   INTEGER_BASE_SIXTEEN_LITERAL,
   STRING_LITERAL,
   CHAR_LITERAL
};

class blaise_token_provider
{
   class blaise_token_provider_constructor : public gasp::common::token_provider_constructor<blaise_token_type>
   {
   public:
      blaise_token_provider_constructor();
   };

   static blaise_token_provider_constructor _private;

public:
   static typename std::vector<blaise_token_type>::const_iterator cbegin();
   static typename std::vector<blaise_token_type>::const_iterator cend();

   static std::string rule(blaise_token_type token_type);
   static std::string name(blaise_token_type token_type);
   static bool keep_value(blaise_token_type token_type);
   static bool keep_token(blaise_token_type token_type);
};

class blaise_token_utility
{
public:
   static bool is_type(blaise_token_type token_type);
   static bool is_unsigned_type(blaise_token_type token_type);
   static bool is_operator(blaise_token_type token_type);
   static bool get_operator_precedence(blaise_token_type token_type);
   static bool is_number(blaise_token_type token_type);
};

std::ostream &
operator<<(std::ostream &os, const blaise_token_type &token_type);

} // namespace gasp::blaise