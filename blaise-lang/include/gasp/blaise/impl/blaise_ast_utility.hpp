#pragma once

#include <memory>

#include <gasp/blaise/ast.hpp>



namespace gasp::blaise::ast {

class blaise_ast_utility {
private:

public:
static std::shared_ptr<blaise_ast_plain_type> as_plain_type(const std::shared_ptr<blaise_ast_type> type); 
static std::shared_ptr<blaise_ast_array_type> as_array_type(const std::shared_ptr<blaise_ast_type> type); 
static std::shared_ptr<const blaise_ast_plain_type> as_plain_type(const std::shared_ptr<const blaise_ast_type> type); 
static std::shared_ptr<const blaise_ast_array_type> as_array_type(const std::shared_ptr<const blaise_ast_type> type); 

static bool is_numeric(std::shared_ptr<blaise_ast_type> type);
static bool is_integer(std::shared_ptr<blaise_ast_type> type);
static bool is_boolean(std::shared_ptr<blaise_ast_type> type);
static bool is_string(std::shared_ptr<blaise_ast_type> type);
static bool is_char(std::shared_ptr<blaise_ast_type> type);
static bool is_array(std::shared_ptr<blaise_ast_type> type);

static std::shared_ptr<blaise_ast_type> get_binary_numeric_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right);
static std::shared_ptr<blaise_ast_type> get_binary_boolean_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right);
static std::shared_ptr<blaise_ast_type> get_binary_char_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right);
static std::shared_ptr<blaise_ast_type> get_binary_string_result(std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right);

static std::shared_ptr<blaise_ast_type> get_resulting_type(const sanelli::token<gasp::blaise::blaise_token_type>& reference, gasp::blaise::blaise_token_type op, std::shared_ptr<blaise_ast_type> operand);
static std::shared_ptr<blaise_ast_type> get_resulting_type(const sanelli::token<gasp::blaise::blaise_token_type>& reference, gasp::blaise::blaise_token_type op, std::shared_ptr<blaise_ast_type> left, std::shared_ptr<blaise_ast_type> right);
static std::shared_ptr<blaise_ast_type> get_common_type(const sanelli::token<gasp::blaise::blaise_token_type>& reference, std::shared_ptr<blaise_ast_type> typeA, std::shared_ptr<blaise_ast_type> typeB);

static bool can_auto_cast(std::shared_ptr<blaise_ast_type> from, std::shared_ptr<blaise_ast_type> to);
static bool can_force_cast(std::shared_ptr<blaise_ast_type> from, std::shared_ptr<blaise_ast_type> to);

};

}