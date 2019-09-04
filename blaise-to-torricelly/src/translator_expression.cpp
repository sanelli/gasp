#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/common/internal_error.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

void blaise_to_torricelly::translator::translate_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                            std::map<std::string, unsigned int> &variables_mapping,
                                                            std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression,
                                                            unsigned int &max_stack_size) const
{
   max_stack_size = 0U;
   switch (expression->expression_type())
   {
   case ast::blaise_ast_expression_type::BINARY:
      break;
   case ast::blaise_ast_expression_type::CAST:
      break;
   case ast::blaise_ast_expression_type::FUNCTION_CALL:
      break;
   case ast::blaise_ast_expression_type::IDENTIFIER:
      break;
   case ast::blaise_ast_expression_type::LITERAL_BOOLEAN:
   {
      auto integer_expression = ast::blaise_ast_expression_utility::as_boolean_literal(expression);
      translate_literal_boolean_expression(torricelly_subroutine, variables_mapping, integer_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_CHAR:
   {
      auto integer_expression = ast::blaise_ast_expression_utility::as_char_literal(expression);
      translate_literal_char_expression(torricelly_subroutine, variables_mapping, integer_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_DOUBLE:
   {
      auto integer_expression = ast::blaise_ast_expression_utility::as_double_literal(expression);
      translate_literal_double_expression(torricelly_subroutine, variables_mapping, integer_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_FLOAT:
   {
      auto integer_expression = ast::blaise_ast_expression_utility::as_float_literal(expression);
      translate_literal_float_expression(torricelly_subroutine, variables_mapping, integer_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_INTEGER:
   {
      auto integer_expression = ast::blaise_ast_expression_utility::as_integer_literal(expression);
      translate_literal_integer_expression(torricelly_subroutine, variables_mapping, integer_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_STRING:
   {
      throw blaise_to_torricelly_internal_error("STRING_LITERAL expression type is not supported yet.");
   }
   break;
   case ast::blaise_ast_expression_type::TERNARY:
      break;
   case ast::blaise_ast_expression_type::UNARY:
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unknown or unsupported expression type found during translation.");
   }
}

void blaise_to_torricelly::translator::translate_literal_integer_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_integer_value> expression, unsigned int &max_stack_size) const
{
   auto initial_value = torricelly_value::make(expression->value());
   auto variable_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::INTEGER), initial_value);
   variables_mapping[sanelli::make_string("^temp", variable_index)] = variable_index;

   auto instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_INTEGER, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
}

void blaise_to_torricelly::translator::translate_literal_float_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_float_value> expression, unsigned int &max_stack_size) const
{
   auto initial_value = torricelly_value::make(expression->value());
   auto variable_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::FLOAT), initial_value);
   variables_mapping[sanelli::make_string("^temp", variable_index)] = variable_index;

   auto instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_FLOAT, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
}

void blaise_to_torricelly::translator::translate_literal_double_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_double_value> expression, unsigned int &max_stack_size) const
{
   auto initial_value = torricelly_value::make(expression->value());
   auto variable_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::DOUBLE), initial_value);
   variables_mapping[sanelli::make_string("^temp", variable_index)] = variable_index;

   auto instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_DOUBLE, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
}

void blaise_to_torricelly::translator::translate_literal_char_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_char_value> expression, unsigned int &max_stack_size) const
{
   auto initial_value = torricelly_value::make(expression->value());
   auto variable_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::CHAR), initial_value);
   variables_mapping[sanelli::make_string("^temp", variable_index)] = variable_index;

   auto instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_CHAR, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
}

void blaise_to_torricelly::translator::translate_literal_boolean_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_boolean_value> expression, unsigned int &max_stack_size) const
{
   auto initial_value = torricelly_value::make(expression->value());
   auto variable_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::BOOLEAN), initial_value);
   variables_mapping[sanelli::make_string("^temp", variable_index)] = variable_index;

   auto instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_BOOLEAN, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
}
