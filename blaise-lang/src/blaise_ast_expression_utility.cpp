#include <memory>

#include <gasp/blaise/ast.hpp>

using namespace gasp::blaise::ast;

std::shared_ptr<blaise_ast_expression_boolean_value> blaise_ast_expression_utility::as_boolean_literal(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_boolean_value>(expression);
}

std::shared_ptr<blaise_ast_expression_string_value> blaise_ast_expression_utility::as_string_literal(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_string_value>(expression);
}

std::shared_ptr<blaise_ast_expression_char_value> blaise_ast_expression_utility::as_char_literal(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_char_value>(expression);
}

std::shared_ptr<blaise_ast_expression_double_value> blaise_ast_expression_utility::as_double_literal(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_double_value>(expression);
}

std::shared_ptr<blaise_ast_expression_float_value> blaise_ast_expression_utility::as_float_literal(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_float_value>(expression);
}

std::shared_ptr<blaise_ast_expression_integer_value> blaise_ast_expression_utility::as_integer_literal(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_integer_value>(expression);
}

std::shared_ptr<blaise_ast_expression_generic_memory_access> blaise_ast_expression_utility::as_memory_access(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_generic_memory_access>(expression);
}

std::shared_ptr<blaise_ast_expression_unary> blaise_ast_expression_utility::as_unary(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_unary>(expression);
}

std::shared_ptr<blaise_ast_expression_binary> blaise_ast_expression_utility::as_binary(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_binary>(expression);
}

std::shared_ptr<blaise_ast_expression_subroutine_call> blaise_ast_expression_utility::as_subroutine_call(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_subroutine_call>(expression);
}

std::shared_ptr<blaise_ast_expression_cast> blaise_ast_expression_utility::as_cast(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_cast>(expression);
}

std::shared_ptr<blaise_ast_expression_ternary> blaise_ast_expression_utility::as_ternary(std::shared_ptr<blaise_ast_expression> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_ternary>(expression);
}

std::shared_ptr<blaise_ast_expression_memory_access> blaise_ast_expression_utility::as_variable_memory_access(std::shared_ptr<blaise_ast_expression_generic_memory_access> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_memory_access>(expression);
}

std::shared_ptr<blaise_ast_expression_array_access> blaise_ast_expression_utility::as_array_memory_access(std::shared_ptr<blaise_ast_expression_generic_memory_access> expression)
{
   return std::static_pointer_cast<blaise_ast_expression_array_access>(expression);
}
