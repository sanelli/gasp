#pragma once

#include <ostream>

namespace gasp::blaise
{
enum class blaise_token : unsigned long
{
   // KEYWORDS
   PROGRAM = 1,
   BEGIN = 2,
   END = 3,
   VAR = 4,

   FUNCTION = 5,
   PROCEDURE = 6,
   RETURN = 7,

   // TYPES
   INTEGER = 100,
   CHAR = 101,
   ARRAY = 102,
   OF = 103,
   STRING = 104,
   FLOAT = 105,
   DOUBLE = 106,

   // PUNCTUATION
   SEMICOLON = 200,
   PERIOD = 201,
   COLON = 202,

   // OTHERS
   IDENTIFIER = 400,
   STRING_CONSTANT = 401,
   INTEGER_CONSTANT = 402,
   CHAR_CONSTANT = 403,
   FLOATING_POINT_CONSTANT = 404
};

const char *get_token_rule(blaise_token token);
const char *get_token_name(blaise_token token);
std::ostream &operator<<(std::ostream &os, const blaise_token &tok);

} // namespace gasp::blaise