#pragma once

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::ast {

enum class blaise_ast_type {
   VOID,
   INTEGER,
   FLOAT,
   DOUBLE,
   CHAR,
   STRING,
   BOOLEAN
};

blaise_ast_type get_type_from_token(const gasp::common::token<gasp::blaise::blaise_token_type>& token);

}