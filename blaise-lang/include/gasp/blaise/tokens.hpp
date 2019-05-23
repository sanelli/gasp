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
   PROGRAM,
   MODULE,
   USES,
   BEGIN,
   END,
   VAR,

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
   SEMICOLON,
   PERIOD,
   COLON,
   COMMA,
  
   // OTHERS
   IDENTIFIER
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

class blaise_token_utility
{
public:
   static bool is_type(blaise_token token);
   static bool is_unsigned_type(blaise_token token);
};

std::ostream &
operator<<(std::ostream &os, const blaise_token &tok);

} // namespace gasp::blaise