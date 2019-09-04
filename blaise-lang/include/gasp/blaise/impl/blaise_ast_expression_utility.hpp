#pragma once

#include <memory>

#include <gasp/blaise/impl/blaise_ast_expression.hpp>

namespace gasp::blaise::ast
{

class blaise_ast_expression_utility
{
public:
   static std::shared_ptr<blaise_ast_expression_boolean_value> as_boolean_literal(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_string_value> as_string_literal(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_char_value> as_char_literal(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_double_value> as_double_literal(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_float_value> as_float_literal(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_integer_value> as_integer_literal(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_generic_memory_access> as_memory_access(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_unary> as_unary(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_binary> as_binary(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_subroutine_call> as_subroutine_call(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_cast> as_cast(std::shared_ptr<blaise_ast_expression> expression);
   static std::shared_ptr<blaise_ast_expression_ternary> as_ternary(std::shared_ptr<blaise_ast_expression> expression);

   static std::shared_ptr<blaise_ast_expression_memory_access> as_variable_memory_access(std::shared_ptr<blaise_ast_expression_generic_memory_access> expression);
   static std::shared_ptr<blaise_ast_expression_array_access> as_array_memory_access(std::shared_ptr<blaise_ast_expression_generic_memory_access> expression);

};

} // namespace gasp::blaise::ast