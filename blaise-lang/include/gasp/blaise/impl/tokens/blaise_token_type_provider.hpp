#pragma once

#include <string>
#include <vector>

#include <gasp/common/tokenizer.hpp>

#include <gasp/blaise/impl/tokens/blaise_token_type.hpp>

namespace gasp::blaise
{

// TODO: Rename into blaise_token_type_provider
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
}