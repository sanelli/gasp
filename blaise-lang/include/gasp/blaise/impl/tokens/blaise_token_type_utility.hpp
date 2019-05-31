#pragma once

#include <gasp/blaise/impl/tokens/blaise_token_type.hpp>

namespace gasp::blaise
{
class blaise_token_type_utility
{
public:
   static bool is_type(blaise_token_type token_type);
   static bool is_unsigned_type(blaise_token_type token_type);
   static bool is_operator(blaise_token_type token_type);
   static bool get_operator_precedence(blaise_token_type token_type);
   static bool is_number(blaise_token_type token_type);
   static bool is_boolean(blaise_token_type token_type);
};

}