#pragma once

#include <string>
#include <vector>

#include <sanelli/sanelli.hpp>
#include <gasp/blaise/tokenizer/impl/blaise_token_type.hpp>

namespace gasp::blaise
{

class blaise_token_type_provider
{
   class blaise_token_provider_constructor : public sanelli::token_provider_constructor<blaise_token_type>
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
   static bool is_punctuation(blaise_token_type token_type);
};
}