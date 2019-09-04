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
   {
      auto memory_access_expression = ast::blaise_ast_expression_utility::as_memory_access(expression);
      translate_memory_access_expression(torricelly_subroutine, variables_mapping, memory_access_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_BOOLEAN:
   {
      auto boolean_expression = ast::blaise_ast_expression_utility::as_boolean_literal(expression);
      translate_literal_boolean_expression(torricelly_subroutine, variables_mapping, boolean_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_CHAR:
   {
      auto char_expression = ast::blaise_ast_expression_utility::as_char_literal(expression);
      translate_literal_char_expression(torricelly_subroutine, variables_mapping, char_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_DOUBLE:
   {
      auto double_expression = ast::blaise_ast_expression_utility::as_double_literal(expression);
      translate_literal_double_expression(torricelly_subroutine, variables_mapping, double_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_FLOAT:
   {
      auto float_expression = ast::blaise_ast_expression_utility::as_float_literal(expression);
      translate_literal_float_expression(torricelly_subroutine, variables_mapping, float_expression, max_stack_size);
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

void blaise_to_torricelly::translator::translate_memory_access_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_generic_memory_access> expression, unsigned int &max_stack_size) const
{
   max_stack_size = 0U;
   switch (expression->memory_access_type())
   {
   case ast::blaise_ast_expression_memory_access_type::ARRAY:
      throw blaise_to_torricelly_internal_error("ARRAY memory access type not supported.");
      break;
   case ast::blaise_ast_expression_memory_access_type::MEMORY_LOCATION:
   {
      auto variable_access_expression = ast::blaise_ast_expression_utility::as_variable_memory_access(expression);
      translate_variable_access_expression(torricelly_subroutine, variables_mapping, variable_access_expression, max_stack_size);
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected or unknown access memory type");
   }
}

void blaise_to_torricelly::translator::translate_variable_access_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_memory_access> expression, unsigned int &max_stack_size) const
{
   auto memory_location = expression->memory_location();
   auto memory_location_type = translate_type(memory_location->type());

   auto memory_location_index_it = variables_mapping.find(memory_location->name());
   if (memory_location_index_it == variables_mapping.end())
      throw blaise_to_torricelly_error(expression->line(), expression->column(), sanelli::make_string("Cannot find variable '", memory_location->name(), "'."));

   auto memory_location_index = memory_location_index_it->second;
   auto instruction_code = torricelly_inst_code::NOOP;
   switch (memory_location_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto memory_location_system_type = torricelly_type_utility::as_system_type(memory_location_type);
      switch (memory_location_system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::BOOLEAN:
         instruction_code = torricelly_inst_code::LOAD_BOOLEAN;
         break;
      case torricelly::torricelly_system_type_type::CHAR:
         instruction_code = torricelly_inst_code::LOAD_CHAR;
         break;
      case torricelly::torricelly_system_type_type::DOUBLE:
         instruction_code = torricelly_inst_code::LOAD_DOUBLE;
         break;
      case torricelly::torricelly_system_type_type::FLOAT:
         instruction_code = torricelly_inst_code::LOAD_FLOAT;
         break;
      case torricelly::torricelly_system_type_type::INTEGER:
         instruction_code = torricelly_inst_code::LOAD_INTEGER;
         break;
      case torricelly::torricelly_system_type_type::STRING_LITERAL:
         throw blaise_to_torricelly_internal_error("STRING_LITERAL torricelly system type when translating memory access expression");
         break;
      case torricelly::torricelly_system_type_type::VOID:
         throw blaise_to_torricelly_internal_error("VOID torricelly system type when translating memory access expression");
         break;
      case torricelly::torricelly_system_type_type::UNDEFINED:
         throw blaise_to_torricelly_internal_error("UNDEFINED torricelly system type when translating memory access expression");
         break;
      default:
         throw blaise_to_torricelly_internal_error("Unexpected or unknown torricelly system type when translating memory access expression");
      }
   }
   break;
   case torricelly_type_type::STRUCTURED:
      throw blaise_to_torricelly_internal_error("Unsupported structured type");
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected or unknown memory location type");
   }
   auto instruction = make_torricelly_instruction(instruction_code, memory_location_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);
   max_stack_size = 1;
}
