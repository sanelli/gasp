#pragma once

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::ast {

class blaise_ast_utility {

public:
static bool is_numeric(blaise_ast_type type);
static bool is_boolean(blaise_ast_type type);
static bool is_string(blaise_ast_type type);
static bool is_char(blaise_ast_type type);

static blaise_ast_type get_binary_numeric_result(blaise_ast_type left, blaise_ast_type right);
static blaise_ast_type get_binary_boolean_result(blaise_ast_type left, blaise_ast_type right);
static blaise_ast_type get_binary_char_result(blaise_ast_type left, blaise_ast_type right);
static blaise_ast_type get_binary_string_result(blaise_ast_type left, blaise_ast_type right);

static blaise_ast_type get_resulting_type(const gasp::common::token<gasp::blaise::blaise_token_type>& token_operator, blaise_ast_type operand);
static blaise_ast_type get_resulting_type(const gasp::common::token<gasp::blaise::blaise_token_type>& token_operator, blaise_ast_type left, blaise_ast_type right);

static bool can_cast(blaise_ast_type from, blaise_ast_type to);

};

}