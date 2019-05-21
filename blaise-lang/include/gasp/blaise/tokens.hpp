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

   // PUNCTUATION
   SEMICOLON = 100,
   PERIOD = 101,

   // OTHERS
   IDENTIFIER = 300

};

const char *get_token_rule(blaise_token token);
const char *get_token_name(blaise_token token);
std::ostream &operator<<(std::ostream &os, const blaise_token &tok);

} // namespace gasp::blaise