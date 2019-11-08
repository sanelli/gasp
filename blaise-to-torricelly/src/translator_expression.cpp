#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <functional>
#include <cmath>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/common/internal_error.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

void blaise_to_torricelly::translator::translate_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                            std::map<std::string, unsigned int> &module_variables_mapping,
                                                            std::map<std::string, unsigned int> &variables_mapping,
                                                            std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression,
                                                            unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_expression" << std::endl);

   max_stack_size = 0U;
   switch (expression->expression_type())
   {
   case ast::blaise_ast_expression_type::BINARY:
   {
      auto binary_expression = ast::blaise_ast_expression_utility::as_binary(expression);
      translate_binary_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, binary_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::CAST:
   {
      auto cast_expression = ast::blaise_ast_expression_utility::as_cast(expression);
      translate_cast_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, cast_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::FUNCTION_CALL:
   {
      auto subroutine_call_expression = ast::blaise_ast_expression_utility::as_subroutine_call(expression);
      translate_subroutine_call_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, subroutine_call_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::IDENTIFIER:
   {
      auto memory_access_expression = ast::blaise_ast_expression_utility::as_memory_access(expression);
      translate_memory_access_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, memory_access_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_BOOLEAN:
   {
      auto boolean_expression = ast::blaise_ast_expression_utility::as_boolean_literal(expression);
      translate_literal_boolean_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, boolean_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_CHAR:
   {
      auto char_expression = ast::blaise_ast_expression_utility::as_char_literal(expression);
      translate_literal_char_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, char_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_DOUBLE:
   {
      auto double_expression = ast::blaise_ast_expression_utility::as_double_literal(expression);
      translate_literal_double_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, double_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_FLOAT:
   {
      auto float_expression = ast::blaise_ast_expression_utility::as_float_literal(expression);
      translate_literal_float_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, float_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_BYTE:
   {
      auto byte_expression = ast::blaise_ast_expression_utility::as_byte_literal(expression);
      translate_literal_byte_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, byte_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_SHORT:
   {
      auto short_expression = ast::blaise_ast_expression_utility::as_short_literal(expression);
      translate_literal_short_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, short_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_INTEGER:
   {
      auto integer_expression = ast::blaise_ast_expression_utility::as_integer_literal(expression);
      translate_literal_integer_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, integer_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_LONG:
   {
      auto long_expression = ast::blaise_ast_expression_utility::as_long_literal(expression);
      translate_literal_long_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, long_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::LITERAL_STRING:
   {
      throw blaise_to_torricelly_internal_error("LITERAL_STRING expression type is not supported yet.");
   }
   break;
   case ast::blaise_ast_expression_type::TERNARY:
   {
      auto ternary_expression = ast::blaise_ast_expression_utility::as_ternary(expression);
      translate_ternary_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, ternary_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::UNARY:
   {
      auto unary_expression = ast::blaise_ast_expression_utility::as_unary(expression);
      translate_unary_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, unary_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_type::NEW:
   {
      auto new_expression = ast::blaise_ast_expression_utility::as_new(expression);
      translate_new_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, new_expression, max_stack_size);
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Unknown or unsupported expression type found during translation.");
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_byte_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_byte_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_byte_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_BYTE, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_byte_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_short_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_short_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_short_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_SHORT, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_short_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_integer_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_integer_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_integer_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_integer_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_long_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_long_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_long_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_LONG, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_long_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_float_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_float_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_float_expression" << std::endl);

   auto initial_value = torricelly_value::make(expression->value());
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_FLOAT, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_float_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_double_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_double_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_double_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_DOUBLE, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_double_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_char_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_char_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_char_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_CHAR, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_char_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_literal_boolean_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_boolean_value> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_literal_boolean_expression" << std::endl);
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(expression->value()));

   auto instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_BOOLEAN, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_literal_boolean_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_memory_access_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_generic_memory_access> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_memory_access_expression" << std::endl);

   max_stack_size = 0U;
   switch (expression->memory_access_type())
   {
   case ast::blaise_ast_expression_memory_access_type::ARRAY:
   {
      auto array_access_expression = ast::blaise_ast_expression_utility::as_array_memory_access(expression);
      translate_array_access_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, array_access_expression, max_stack_size);
   }
   break;
   case ast::blaise_ast_expression_memory_access_type::MEMORY_LOCATION:
   {
      auto variable_access_expression = ast::blaise_ast_expression_utility::as_variable_memory_access(expression);
      translate_variable_access_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, variable_access_expression, max_stack_size);
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error(sanelli::make_string("Unexpected or unknown access memory type (id = ", (int)expression->memory_access_type(), ")"));
   }
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_memory_access_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_variable_access_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_memory_access> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_variable_access_expression" << std::endl);
   auto memory_location = expression->memory_location();

   auto memory_location_index_it = variables_mapping.find(memory_location->name());
   if (memory_location_index_it == variables_mapping.end())
      throw blaise_to_torricelly_error(expression->line(), expression->column(), sanelli::make_string("Cannot find variable '", memory_location->name(), "'."));

   auto memory_location_index = memory_location_index_it->second;
   auto instruction_code = (memory_location->type()->type_type() == blaise::ast::blaise_ast_type_type::ARRAY)
                               ? torricelly_inst_code::LOAD_ARRAY // When passing an array I have a memory access for a type ARRAY
                               : compute_instruction_code(memory_location->type(),
                                                          torricelly_inst_code::LOAD_BYTE, torricelly_inst_code::LOAD_SHORT, torricelly_inst_code::LOAD_INTEGER, torricelly_inst_code::LOAD_LONG,
                                                          torricelly_inst_code::LOAD_FLOAT, torricelly_inst_code::LOAD_DOUBLE,
                                                          torricelly_inst_code::LOAD_CHAR, torricelly_inst_code::LOAD_BOOLEAN);
   auto instruction = torricelly_instruction::make(instruction_code, memory_location_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);
   max_stack_size = 1;
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_variable_access_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_array_access_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_array_access> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_array_access_expression" << std::endl);

   auto array_variable = expression->array();

   // Get the index of the array
   auto array_variable_index_it = variables_mapping.find(array_variable->name());
   if (array_variable_index_it == variables_mapping.end())
      throw blaise_to_torricelly_error(expression->line(), expression->column(), sanelli::make_string("Cannot find variable '", array_variable->name(), "'."));
   auto variable_index = array_variable_index_it->second;

   // Traslate indexing expression
   auto expression_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->indexing(), expression_max_stack_size);

   // Add the number of dimnsions
   // At present only one because blaise do not support multiple arrays
   auto dimensions = 1;
   auto size_value_variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(1));
   auto load_size_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, size_value_variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_size_instruction);

   // Generate the load array instruction
   max_stack_size = std::max(max_stack_size, 2U);
   auto array_type = gasp::blaise::ast::blaise_ast_utility::as_array_type(array_variable->type());
   auto load_instruction_code = compute_instruction_code(expression->result_type(),
                                                         torricelly_inst_code::LOAD_ARRAY_BYTE, torricelly_inst_code::LOAD_ARRAY_SHORT, torricelly_inst_code::LOAD_ARRAY_INTEGER, torricelly_inst_code::LOAD_ARRAY_LONG,
                                                         torricelly_inst_code::LOAD_ARRAY_FLOAT, torricelly_inst_code::LOAD_ARRAY_DOUBLE,
                                                         torricelly_inst_code::LOAD_ARRAY_CHAR, torricelly_inst_code::LOAD_ARRAY_BOOLEAN);

   auto load_instruction = torricelly_instruction::make(load_instruction_code, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_instruction);
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_array_access_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_unary_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_unary> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_unary_expression" << std::endl);
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->operand(), max_stack_size);
   auto instruction_code = torricelly_inst_code::NOOP;

   switch (expression->op())
   {
   case gasp::blaise::blaise_token_type::LOGICAL_NOT:
      instruction_code = torricelly_inst_code::NOT;
      break;
   case gasp::blaise::blaise_token_type::MINUS:
      instruction_code = compute_instruction_code(expression->result_type(),
                                                  torricelly_inst_code::NEGATE_BYTE, torricelly_inst_code::NEGATE_SHORT, torricelly_inst_code::NEGATE_INTEGER, torricelly_inst_code::NEGATE_LONG,
                                                  torricelly_inst_code::NEGATE_FLOAT, torricelly_inst_code::NEGATE_DOUBLE);
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unknown or unexpected unary operator");
   }
   auto instruction = torricelly_instruction::make(instruction_code);
   torricelly_subroutine->append_instruction(instruction);
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_unary_expression" << std::endl);
}

torricelly::torricelly_inst_code blaise_to_torricelly::translator::compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type,
                                                                                            torricelly::torricelly_inst_code byte_op,
                                                                                            torricelly::torricelly_inst_code short_op,
                                                                                            torricelly::torricelly_inst_code integer_op,
                                                                                            torricelly::torricelly_inst_code long_op) const
{
   auto torricelly_type = translate_type(blaise_type);
   switch (torricelly_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto memory_location_system_type = torricelly_type_utility::as_system_type(torricelly_type);
      switch (memory_location_system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::BYTE:
         return byte_op;
      case torricelly::torricelly_system_type_type::SHORT:
         return short_op;
      case torricelly::torricelly_system_type_type::INTEGER:
         return integer_op;
      case torricelly::torricelly_system_type_type::LONG:
         return long_op;
      case torricelly::torricelly_system_type_type::DOUBLE:
         throw blaise_to_torricelly_internal_error("DOUBLE torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::FLOAT:
         throw blaise_to_torricelly_internal_error("FLOAT torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::BOOLEAN:
         throw blaise_to_torricelly_internal_error("BOOLEAN torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::CHAR:
         throw blaise_to_torricelly_internal_error("CHAR torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::LITERAL_STRING:
         throw blaise_to_torricelly_internal_error("LITERAL_STRING torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::VOID:
         throw blaise_to_torricelly_internal_error("VOID torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::UNDEFINED:
         throw blaise_to_torricelly_internal_error("UNDEFINED torricelly type unsupported.");
      default:
         throw blaise_to_torricelly_internal_error("Unexpected or unknown torricelly");
      }
   }
   case torricelly_type_type::STRUCTURED:
      throw blaise_to_torricelly_internal_error("Unsupported structured type.");
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected or unknown type when computing instruction code for numeric.");
   }
}

torricelly_inst_code blaise_to_torricelly::translator::compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type,
                                                                                torricelly::torricelly_inst_code byte_op,
                                                                                torricelly::torricelly_inst_code short_op,
                                                                                torricelly::torricelly_inst_code integer_op,
                                                                                torricelly::torricelly_inst_code long_op,
                                                                                torricelly::torricelly_inst_code float_op,
                                                                                torricelly::torricelly_inst_code double_op) const
{
   auto torricelly_type = translate_type(blaise_type);
   switch (torricelly_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto memory_location_system_type = torricelly_type_utility::as_system_type(torricelly_type);
      switch (memory_location_system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::BYTE:
         return byte_op;
      case torricelly::torricelly_system_type_type::SHORT:
         return short_op;
      case torricelly::torricelly_system_type_type::INTEGER:
         return integer_op;
      case torricelly::torricelly_system_type_type::LONG:
         return long_op;
      case torricelly::torricelly_system_type_type::DOUBLE:
         return double_op;
      case torricelly::torricelly_system_type_type::FLOAT:
         return float_op;
      case torricelly::torricelly_system_type_type::BOOLEAN:
         throw blaise_to_torricelly_internal_error("BOOLEAN torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::CHAR:
         throw blaise_to_torricelly_internal_error("CHAR torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::LITERAL_STRING:
         throw blaise_to_torricelly_internal_error("LITERAL_STRING torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::VOID:
         throw blaise_to_torricelly_internal_error("VOID torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::UNDEFINED:
         throw blaise_to_torricelly_internal_error("UNDEFINED torricelly type unsupported.");
      default:
         throw blaise_to_torricelly_internal_error("Unexpected or unknown torricelly");
      }
   }
   case torricelly_type_type::STRUCTURED:
      throw blaise_to_torricelly_internal_error("Unsupported structured type.");
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected or unknown type when computing instruction code for numeric.");
   }
}

torricelly::torricelly_inst_code blaise_to_torricelly::translator::compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type,
                                                                                            torricelly::torricelly_inst_code byte_op,
                                                                                            torricelly::torricelly_inst_code short_op,
                                                                                            torricelly::torricelly_inst_code integer_op,
                                                                                            torricelly::torricelly_inst_code long_op,
                                                                                            torricelly::torricelly_inst_code float_op,
                                                                                            torricelly::torricelly_inst_code double_op,
                                                                                            torricelly::torricelly_inst_code char_op,
                                                                                            torricelly::torricelly_inst_code boolean_op) const
{
   auto torricelly_type = translate_type(blaise_type);
   switch (torricelly_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto memory_location_system_type = torricelly_type_utility::as_system_type(torricelly_type);
      switch (memory_location_system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::BYTE:
         return byte_op;
      case torricelly::torricelly_system_type_type::SHORT:
         return short_op;
      case torricelly::torricelly_system_type_type::INTEGER:
         return integer_op;
      case torricelly::torricelly_system_type_type::LONG:
         return long_op;
      case torricelly::torricelly_system_type_type::DOUBLE:
         return double_op;
      case torricelly::torricelly_system_type_type::FLOAT:
         return float_op;
      case torricelly::torricelly_system_type_type::BOOLEAN:
         return boolean_op;
      case torricelly::torricelly_system_type_type::CHAR:
         return char_op;
      case torricelly::torricelly_system_type_type::LITERAL_STRING:
         throw blaise_to_torricelly_internal_error("LITERAL_STRING torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::VOID:
         throw blaise_to_torricelly_internal_error("VOID torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::UNDEFINED:
         throw blaise_to_torricelly_internal_error("UNDEFINED torricelly type unsupported.");
      default:
         throw blaise_to_torricelly_internal_error("Unexpected or unknown torricelly");
      }
   }
   case torricelly_type_type::STRUCTURED:
      throw blaise_to_torricelly_internal_error("Unsupported structured type.");
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected or unknown type when computing instruction code.");
   }
}

torricelly_inst_code blaise_to_torricelly::translator::get_jump_for_comparison_token(gasp::blaise::blaise_token_type token_type) const
{
   switch (token_type)
   {
   case gasp::blaise::blaise_token_type::LESS_THAN:
      return torricelly_inst_code::JUMP_LT_ZERO;
   case gasp::blaise::blaise_token_type::EQUAL_TO:
      return torricelly_inst_code::JUMP_EQ_ZERO;
   case gasp::blaise::blaise_token_type::GREAT_THAN:
      return torricelly_inst_code::JUMP_GT_ZERO;
   case gasp::blaise::blaise_token_type::GREAT_THAN_OR_EQUAL_TO:
      return torricelly_inst_code::JUMP_GTE_ZERO;
   case gasp::blaise::blaise_token_type::LESS_THAN_OR_EQUAL_TO:
      return torricelly_inst_code::JUMP_LTE_ZERO;
   case gasp::blaise::blaise_token_type::NOT_EQUAL_TO:
      return torricelly_inst_code::JUMP_NOT_ZERO;
   default:
      throw blaise_to_torricelly_internal_error("Cannot get blaise token type integer comparin value: unsupported token type.");
   }
}

void blaise_to_torricelly::translator::translate_binary_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_binary> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_binary_expression" << std::endl);
   auto left_expression_max_stack_size = 0U;
   auto right_expression_max_stack_size = 0U;

   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->left(), left_expression_max_stack_size);
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->right(), right_expression_max_stack_size);

   max_stack_size = 1 + std::max(left_expression_max_stack_size, right_expression_max_stack_size);

   auto token_type = expression->op();
   switch (token_type)
   {
   case gasp::blaise::blaise_token_type::LOGICAL_AND:
   {
      auto instruction = torricelly_instruction::make(torricelly_inst_code::AND);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::LOGICAL_OR:
   {
      auto instruction = torricelly_instruction::make(torricelly_inst_code::OR);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::LESS_THAN:
   case gasp::blaise::blaise_token_type::EQUAL_TO:
   case gasp::blaise::blaise_token_type::GREAT_THAN:
   case gasp::blaise::blaise_token_type::GREAT_THAN_OR_EQUAL_TO:
   case gasp::blaise::blaise_token_type::LESS_THAN_OR_EQUAL_TO:
   case gasp::blaise::blaise_token_type::NOT_EQUAL_TO:
   {
      //          CMP_XXX
      //          JUMP_XXX_ZERO [on_true]
      //          PUSH_BOOLEAN [false]
      //          JUMP [done]
      // on_true: PUSH_BOOLEAN [true]
      // done:    NOOP

      auto comparison_instruction_code = compute_instruction_code(expression->left()->result_type(),
                                                                  torricelly_inst_code::CMP_BYTE, torricelly_inst_code::CMP_SHORT, torricelly_inst_code::CMP_INTEGER, torricelly_inst_code::CMP_LONG,
                                                                  torricelly_inst_code::CMP_FLOAT, torricelly_inst_code::CMP_DOUBLE, torricelly_inst_code::CMP_CHAR,
                                                                  torricelly_inst_code::CMP_BOOLEAN);
      auto on_true_label = torricelly_subroutine->next_label();
      auto on_done_label = torricelly_subroutine->next_label();

      auto true_value_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(true));
      auto false_value_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(false));

      auto comparison_instruction = torricelly_instruction::make(comparison_instruction_code);
      torricelly_subroutine->append_instruction(comparison_instruction);

      auto jump_instruction_code = get_jump_for_comparison_token(token_type);
      auto jump_instruction = torricelly_instruction::make(jump_instruction_code, on_true_label, torricelly_inst_ref_type::LABEL);
      torricelly_subroutine->append_instruction(jump_instruction);

      auto load_false_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_BOOLEAN, false_value_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(load_false_instruction);

      auto jump_to_done_instruction = torricelly_instruction::make(torricelly_inst_code::JUMP, on_done_label, torricelly_inst_ref_type::LABEL);
      torricelly_subroutine->append_instruction(jump_to_done_instruction);

      auto load_true_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_BOOLEAN, true_value_index, torricelly_inst_ref_type::SUBROUTINE);
      load_true_instruction.set_label(on_true_label);
      torricelly_subroutine->append_instruction(load_true_instruction);

      auto noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
      noop.set_label(on_done_label);
      torricelly_subroutine->append_instruction(noop);
   }

   break;
   case gasp::blaise::blaise_token_type::PLUS:
   {
      auto instruction_code = compute_instruction_code(expression->left()->result_type(),
                                                       torricelly_inst_code::ADD_BYTE, torricelly_inst_code::ADD_SHORT, torricelly_inst_code::ADD_INTEGER, torricelly_inst_code::ADD_LONG,
                                                       torricelly_inst_code::ADD_FLOAT, torricelly_inst_code::ADD_DOUBLE);
      auto instruction = torricelly_instruction::make(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::MINUS:
   {
      auto instruction_code = compute_instruction_code(expression->left()->result_type(),
                                                       torricelly_inst_code::SUBTRACT_BYTE, torricelly_inst_code::SUBTRACT_SHORT, torricelly_inst_code::SUBTRACT_INTEGER, torricelly_inst_code::SUBTRACT_LONG,
                                                       torricelly_inst_code::SUBTRACT_FLOAT, torricelly_inst_code::SUBTRACT_DOUBLE);
      auto instruction = torricelly_instruction::make(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::MULTIPLY:
   {
      auto instruction_code = compute_instruction_code(expression->left()->result_type(),
                                                       torricelly_inst_code::MULTIPLY_BYTE, torricelly_inst_code::MULTIPLY_SHORT, torricelly_inst_code::MULTIPLY_INTEGER, torricelly_inst_code::MULTIPLY_LONG,
                                                       torricelly_inst_code::MULTIPLY_FLOAT, torricelly_inst_code::MULTIPLY_DOUBLE);
      auto instruction = torricelly_instruction::make(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::DIVIDE:
   {
      auto instruction_code = compute_instruction_code(expression->left()->result_type(),
                                                       torricelly_inst_code::DIVIDE_BYTE, torricelly_inst_code::DIVIDE_SHORT, torricelly_inst_code::DIVIDE_INTEGER, torricelly_inst_code::DIVIDE_LONG,
                                                       torricelly_inst_code::DIVIDE_FLOAT, torricelly_inst_code::DIVIDE_DOUBLE);
      auto instruction = torricelly_instruction::make(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::REMAINDER:
   {
      auto instruciton_code = compute_instruction_code(expression->left()->result_type(),
                                                       torricelly_inst_code::REMINDER_BYTE, torricelly_inst_code::REMINDER_SHORT,
                                                       torricelly_inst_code::REMINDER_INTEGER, torricelly_inst_code::REMINDER_LONG);
      auto instruction = torricelly_instruction::make(instruciton_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   default:
      throw blaise_to_torricelly_error(expression->line(), expression->column(), "Unknown or unexpected binary operator.");
   }
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_binary_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_ternary_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_ternary> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_ternary_expression" << std::endl);

   //          <condition>
   //          LOAD_BOOLEAN [true]
   //          CMP_BOOLEAN
   //          JUMP_EQ_ZERO [on_true]
   //          <else_instructions>
   //          JUMP [done]
   // on_true: NOOP
   //          <then_instructions>
   // done:    NOOP

   auto condition_max_stack_size = 0U;
   translate_condition(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->condition(), condition_max_stack_size);

   auto on_true_label = torricelly_subroutine->next_label();
   auto on_done_label = torricelly_subroutine->next_label();

   auto jump_eq_zero_instrution = torricelly_instruction::make(torricelly_inst_code::JUMP_EQ_ZERO, on_true_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_eq_zero_instrution);

   auto else_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->else_expression(), condition_max_stack_size);

   auto jump_to_done = torricelly_instruction::make(torricelly_inst_code::JUMP, on_done_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_to_done);

   auto on_true_noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
   on_true_noop.set_label(on_true_label);
   torricelly_subroutine->append_instruction(on_true_noop);

   auto then_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->then_expression(), then_max_stack_size);

   auto on_done_noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
   on_done_noop.set_label(on_done_label);
   torricelly_subroutine->append_instruction(on_done_noop);

   max_stack_size = std::max({(1 + condition_max_stack_size), then_max_stack_size, else_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_ternary_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_subroutine_call_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                            std::map<std::string, unsigned int> &module_variables_mapping,
                                                                            std::map<std::string, unsigned int> &variables_mapping,
                                                                            std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_subroutine_call> expression,
                                                                            unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_subroutine_call_expression" << std::endl);
   auto subroutine = expression->subroutine();
   translate_subroutine_call(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping,
                             subroutine,
                             [expression](unsigned int index) { return expression->get_parameter(index); },
                             max_stack_size);

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_subroutine_call_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_cast_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping,
                                                                 std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_cast> expression, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_cast_expression" << std::endl);

   auto operand_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression->operand(), operand_max_stack_size);

   max_stack_size = std::max(1U, operand_max_stack_size);

   auto source_type = translate_type(expression->operand()->result_type());
   auto target_type = translate_type(expression->result_type());

   if (source_type->type_type() != torricelly_type_type::SYSTEM)
      throw blaise_to_torricelly_internal_error("Cannot create a cast expression where the source type is not a system type.");
   if (target_type->type_type() != torricelly_type_type::SYSTEM)
      throw blaise_to_torricelly_internal_error("Cannot create a cast expression where the target type is not a system type.");

   auto source_system_type = torricelly_type_utility::as_system_type(source_type);
   auto target_system_type = torricelly_type_utility::as_system_type(target_type);

   auto instruction_code = torricelly_inst_code::NOOP;
   switch (source_system_type->system_type())
   {
   case torricelly::torricelly_system_type_type::INTEGER:
   {
      if (target_system_type->system_type() == torricelly::torricelly_system_type_type::BOOLEAN)
         instruction_code = torricelly_inst_code::CAST_INT_BOOLEAN;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::CHAR)
         instruction_code = torricelly_inst_code::CAST_INT_CHAR;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::FLOAT)
         instruction_code = torricelly_inst_code::CAST_INT_FLOAT;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::DOUBLE)
         instruction_code = torricelly_inst_code::CAST_INT_DOUBLE;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::INTEGER)
         instruction_code = torricelly_inst_code::NOOP;
      else
         throw blaise_to_torricelly_internal_error("Target system type cannot be casted from INTEGER type.");
   }
   break;
   case torricelly::torricelly_system_type_type::BOOLEAN:
   {
      if (target_system_type->system_type() == torricelly::torricelly_system_type_type::INTEGER)
         instruction_code = torricelly_inst_code::CAST_BOOLEAN_INT;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::BOOLEAN)
         instruction_code = torricelly_inst_code::NOOP;
      else
         throw blaise_to_torricelly_internal_error("Target system type cannot be casted from BOOLEAN type.");
   }
   break;
   case torricelly::torricelly_system_type_type::CHAR:
   {
      if (target_system_type->system_type() == torricelly::torricelly_system_type_type::INTEGER)
         instruction_code = torricelly_inst_code::CAST_CHAR_INT;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::CHAR)
         instruction_code = torricelly_inst_code::NOOP;
      else
         throw blaise_to_torricelly_internal_error("Target system type cannot be casted from CHAR type.");
   }
   break;
   case torricelly::torricelly_system_type_type::FLOAT:
   {
      if (target_system_type->system_type() == torricelly::torricelly_system_type_type::INTEGER)
         instruction_code = torricelly_inst_code::CAST_FLOAT_INT;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::DOUBLE)
         instruction_code = torricelly_inst_code::CAST_FLOAT_DOUBLE;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::FLOAT)
         instruction_code = torricelly_inst_code::NOOP;
      else
         throw blaise_to_torricelly_internal_error("Target system type cannot be casted from FLOAT type.");
   }
   break;
   case torricelly::torricelly_system_type_type::DOUBLE:
   {
      if (target_system_type->system_type() == torricelly::torricelly_system_type_type::INTEGER)
         instruction_code = torricelly_inst_code::CAST_DOUBLE_INT;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::FLOAT)
         instruction_code = torricelly_inst_code::CAST_DOUBLE_FLOAT;
      else if (target_system_type->system_type() == torricelly::torricelly_system_type_type::DOUBLE)
         instruction_code = torricelly_inst_code::NOOP;
      else
         throw blaise_to_torricelly_internal_error("Target system type cannot be casted from DOUBLE type.");
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Source system type cannot be casted to any other type.");
   }

   auto cast_instruction = torricelly_instruction::make(instruction_code);
   torricelly_subroutine->append_instruction(cast_instruction);

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_cast_expression" << std::endl);
}

void blaise_to_torricelly::translator::translate_new_expression(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,
                                                                std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                std::map<std::string, unsigned int> &module_variables_mapping,
                                                                std::map<std::string, unsigned int> &variables_mapping,
                                                                std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_new> expression,
                                                                unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_new_expression" << std::endl);

   auto array_type = ast::blaise_ast_utility::as_array_type(expression->result_type());
   auto array_underlying_type = array_type->underlying_type();

   max_stack_size = 2U;

   // Translate expressions from left to right
   for (auto parameter_index = 0; parameter_index < expression->count_parameters(); ++parameter_index)
   {
      unsigned int expression_max_stack_size;
      auto parameter_expression = expression->get_parameter(parameter_index);
      translate_expression(torricelly_module, torricelly_subroutine,
                           module_variables_mapping, variables_mapping,
                           parameter_expression, expression_max_stack_size);
      max_stack_size = std::max(max_stack_size, expression_max_stack_size);
   }

   // Push on the stack the number of dimensions
   auto variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping,
                                       torricelly_value::make((int)expression->count_parameters()));
   auto load_dimensions_instruction = torricelly_instruction::make(
       torricelly_inst_code::LOAD_INTEGER, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_dimensions_instruction);

   // Push on the stack the initial value for the content of the array
   auto initial_value = get_type_initial_value(array_underlying_type);
   auto variable_initial_value_index = add_temporary(torricelly_module, torricelly_subroutine,
                                                     variables_mapping, initial_value);
   auto load_initial_value_inst_code = compute_instruction_code(array_underlying_type,
                                                                torricelly_inst_code::LOAD_BYTE, torricelly_inst_code::LOAD_SHORT, torricelly_inst_code::LOAD_INTEGER, torricelly_inst_code::LOAD_LONG,
                                                                torricelly_inst_code::LOAD_FLOAT, torricelly_inst_code::LOAD_DOUBLE,
                                                                torricelly_inst_code::LOAD_CHAR, torricelly_inst_code::LOAD_BOOLEAN);
   auto load_initial_value_instruction = torricelly_instruction::make(load_initial_value_inst_code, variable_initial_value_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_initial_value_instruction);

   // Push on the stack the value true to indicate we are passing the initial value
   auto variable_have_initial_value = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping,
                                                    torricelly_value::make(true));
   auto load_has_initial_value = torricelly_instruction::make(
       torricelly_inst_code::LOAD_BOOLEAN, variable_have_initial_value, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_has_initial_value);

   // Create the ALLOCATE instruction
   auto allocate_instrution_code = compute_instruction_code(array_underlying_type,
                                                            torricelly_inst_code::ALLOCATE_BYTE_ARRAY, torricelly_inst_code::ALLOCATE_SHORT_ARRAY, torricelly_inst_code::ALLOCATE_INT_ARRAY, torricelly_inst_code::ALLOCATE_LONG_ARRAY, 
                                                            torricelly_inst_code::ALLOCATE_FLOAT_ARRAY, torricelly_inst_code::ALLOCATE_DOUBLE_ARRAY, 
                                                            torricelly_inst_code::ALLOCATE_CHAR_ARRAY, torricelly_inst_code::ALLOCATE_BOOLEAN_ARRAY);
   auto allocate_instruction = torricelly_instruction::make(allocate_instrution_code);
   torricelly_subroutine->append_instruction(allocate_instruction);

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_new_expression" << std::endl);
}
