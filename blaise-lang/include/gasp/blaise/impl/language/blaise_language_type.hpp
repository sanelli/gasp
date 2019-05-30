#pragma once

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::language {

enum class blaise_language_type {
   VOID,
   INTEGER,
   FLOAT,
   DOUBLE,
   CHAR,
   STRING,
   BOOLEAN
};

blaise_language_type get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token);

}