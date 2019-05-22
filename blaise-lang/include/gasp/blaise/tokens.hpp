#pragma once

#include <ostream>
#include <map>
#include <string>

#include <gasp/common/tokenizer.hpp>

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
};

std::ostream &operator<<(std::ostream &os, const blaise_token &tok);

} // namespace gasp::blaise