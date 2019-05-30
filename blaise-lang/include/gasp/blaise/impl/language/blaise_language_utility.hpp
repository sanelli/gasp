#pragma once

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::language {

class blaise_language_utility {

public:
static bool is_numeric(blaise_expression_type type);
static bool is_boolean(blaise_expression_type type);
static bool is_string(blaise_expression_type type);
static bool is_char(blaise_expression_type type);

static blaise_expression_type get_binary_numeric_result(blaise_expression_type left, blaise_expression_type right);
static blaise_expression_type get_binary_boolean_result(blaise_expression_type left, blaise_expression_type right);
static blaise_expression_type get_binary_char_result(blaise_expression_type left, blaise_expression_type right);
static blaise_expression_type get_binary_string_result(blaise_expression_type left, blaise_expression_type right);

static blaise_expression_type get_resulting_type(const gasp::common::token<gasp::blaise::blaise_token_type>& token_operator, blaise_expression_type operand);
static blaise_expression_type get_resulting_type(const gasp::common::token<gasp::blaise::blaise_token_type>& token_operator, blaise_expression_type left, blaise_expression_type right);

};

}