#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
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
   {
      auto binary_expression = ast::blaise_ast_expression_utility::as_binary(expression);
      translate_binary_expression(torricelly_subroutine, variables_mapping, binary_expression, max_stack_size);
   }
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
   {
      auto unary_expression = ast::blaise_ast_expression_utility::as_unary(expression);
      translate_unary_expression(torricelly_subroutine, variables_mapping, unary_expression, max_stack_size);
   }
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

   auto memory_location_index_it = variables_mapping.find(memory_location->name());
   if (memory_location_index_it == variables_mapping.end())
      throw blaise_to_torricelly_error(expression->line(), expression->column(), sanelli::make_string("Cannot find variable '", memory_location->name(), "'."));

   auto memory_location_index = memory_location_index_it->second;
   auto instruction_code = compute_instruction_code(memory_location->type(), torricelly_inst_code::LOAD_INTEGER,
                                                    torricelly_inst_code::LOAD_FLOAT, torricelly_inst_code::LOAD_DOUBLE, torricelly_inst_code::LOAD_CHAR,
                                                    torricelly_inst_code::LOAD_BOOLEAN);
   auto instruction = make_torricelly_instruction(instruction_code, memory_location_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);
   max_stack_size = 1;
}

void blaise_to_torricelly::translator::translate_unary_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_unary> expression, unsigned int &max_stack_size) const
{
   translate_expression(torricelly_subroutine, variables_mapping, expression->operand(), max_stack_size);
   auto instruction_code = torricelly_inst_code::NOOP;

   switch (expression->op())
   {
   case gasp::blaise::blaise_token_type::LOGICAL_NOT:
      instruction_code = torricelly_inst_code::NOT;
      break;
   case gasp::blaise::blaise_token_type::MINUS:
      instruction_code = compute_instruction_code(expression->result_type(), torricelly_inst_code::NEGATE_INTEGER, torricelly_inst_code::NEGATE_FLOAT, torricelly_inst_code::NEGATE_DOUBLE);
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unknown or unexpected unary operator");
   }
   auto instruction = make_torricelly_instruction(instruction_code);
   torricelly_subroutine->append_instruction(instruction);
}

torricelly_inst_code blaise_to_torricelly::translator::compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type, torricelly_inst_code integer_op, torricelly_inst_code float_op, torricelly_inst_code double_op) const
{
   auto torricelly_type = translate_type(blaise_type);
   switch (torricelly_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto memory_location_system_type = torricelly_type_utility::as_system_type(torricelly_type);
      switch (memory_location_system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::DOUBLE:
         return double_op;
      case torricelly::torricelly_system_type_type::FLOAT:
         return float_op;
      case torricelly::torricelly_system_type_type::INTEGER:
         return integer_op;
      case torricelly::torricelly_system_type_type::BOOLEAN:
         throw blaise_to_torricelly_internal_error("BOOLEAN torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::CHAR:
         throw blaise_to_torricelly_internal_error("CHAR torricelly type unsupported.");
      case torricelly::torricelly_system_type_type::STRING_LITERAL:
         throw blaise_to_torricelly_internal_error("STRING_LITERAL torricelly type unsupported.");
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
      throw blaise_to_torricelly_internal_error("Unexpected or unknown type.");
   }
}

torricelly::torricelly_inst_code blaise_to_torricelly::translator::compute_instruction_code(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> blaise_type, torricelly::torricelly_inst_code integer_op, torricelly::torricelly_inst_code float_op, torricelly::torricelly_inst_code double_op,
                                                                                            torricelly::torricelly_inst_code char_op, torricelly::torricelly_inst_code boolean_op) const
{
   auto torricelly_type = translate_type(blaise_type);
   switch (torricelly_type->type_type())
   {
   case torricelly_type_type::SYSTEM:
   {
      auto memory_location_system_type = torricelly_type_utility::as_system_type(torricelly_type);
      switch (memory_location_system_type->system_type())
      {
      case torricelly::torricelly_system_type_type::DOUBLE:
         return double_op;
      case torricelly::torricelly_system_type_type::FLOAT:
         return float_op;
      case torricelly::torricelly_system_type_type::INTEGER:
         return integer_op;
      case torricelly::torricelly_system_type_type::BOOLEAN:
         return boolean_op;
      case torricelly::torricelly_system_type_type::CHAR:
         return char_op;
      case torricelly::torricelly_system_type_type::STRING_LITERAL:
         throw blaise_to_torricelly_internal_error("STRING_LITERAL torricelly type unsupported.");
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
      throw blaise_to_torricelly_internal_error("Unexpected or unknown type.");
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

void blaise_to_torricelly::translator::translate_binary_expression(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression_binary> expression, unsigned int &max_stack_size) const
{
   auto left_expression_max_stack_size = 0U;
   auto right_expression_max_stack_size = 0U;

   translate_expression(torricelly_subroutine, variables_mapping, expression->left(), left_expression_max_stack_size);
   translate_expression(torricelly_subroutine, variables_mapping, expression->right(), right_expression_max_stack_size);

   max_stack_size = 1 + std::max(left_expression_max_stack_size, right_expression_max_stack_size);

   auto token_type = expression->op();
   switch (token_type)
   {
   case gasp::blaise::blaise_token_type::LOGICAL_AND:
   {
      auto instruction = make_torricelly_instruction(torricelly_inst_code::AND);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::LOGICAL_OR:
   {
      auto instruction = make_torricelly_instruction(torricelly_inst_code::OR);
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

      auto comparison_instruction_code = compute_instruction_code(expression->result_type(), torricelly_inst_code::CMP_INTEGER,
                                                                  torricelly_inst_code::CMP_FLOAT, torricelly_inst_code::CMP_DOUBLE, torricelly_inst_code::CMP_CHAR,
                                                                  torricelly_inst_code::CMP_BOOLEAN);
      auto on_true_label = torricelly_subroutine->next_label();
      auto on_done_label = torricelly_subroutine->next_label();

      auto true_value_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::BOOLEAN), torricelly_value::make(true));
      auto false_value_index = torricelly_subroutine->add_variable(make_torricelly_system_type(torricelly_system_type_type::BOOLEAN), torricelly_value::make(false));

      auto comparison_instruction = make_torricelly_instruction(comparison_instruction_code);
      torricelly_subroutine->append_instruction(comparison_instruction);

      auto jump_instruction_code = get_jump_for_comparison_token(token_type);
      auto jump_instruction = make_torricelly_instruction(jump_instruction_code, on_true_label, torricelly_inst_ref_type::LABEL);
      torricelly_subroutine->append_instruction(jump_instruction);

      auto load_false_instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_BOOLEAN, false_value_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(load_false_instruction);

      auto jump_to_done_instruction = make_torricelly_instruction(torricelly_inst_code::JUMP, on_done_label, torricelly_inst_ref_type::LABEL);
      torricelly_subroutine->append_instruction(jump_to_done_instruction);

      auto load_true_instruction = make_torricelly_instruction(torricelly_inst_code::LOAD_BOOLEAN, true_value_index, torricelly_inst_ref_type::SUBROUTINE);
      load_true_instruction->set_label(on_true_label);
      torricelly_subroutine->append_instruction(load_true_instruction);

      auto noop = make_torricelly_instruction(torricelly_inst_code::NOOP);
      noop->set_label(on_done_label);
      torricelly_subroutine->append_instruction(noop);
   }

   break;
   case gasp::blaise::blaise_token_type::PLUS:
   {
      auto instruction_code = compute_instruction_code(expression->result_type(), torricelly_inst_code::ADD_INTEGER, torricelly_inst_code::ADD_FLOAT, torricelly_inst_code::ADD_DOUBLE);
      auto instruction = make_torricelly_instruction(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::MINUS:
   {
      auto instruction_code = compute_instruction_code(expression->result_type(), torricelly_inst_code::SUBTRACT_INTEGER, torricelly_inst_code::SUBTRACT_FLOAT, torricelly_inst_code::SUBTRACT_DOUBLE);
      auto instruction = make_torricelly_instruction(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::MULTIPLY:
   {
      auto instruction_code = compute_instruction_code(expression->result_type(), torricelly_inst_code::MULTIPLY_INTEGER, torricelly_inst_code::MULTIPLY_FLOAT, torricelly_inst_code::MULTIPLY_DOUBLE);
      auto instruction = make_torricelly_instruction(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::DIVIDE:
   {
      auto instruction_code = compute_instruction_code(expression->result_type(), torricelly_inst_code::DIVIDE_INTEGER, torricelly_inst_code::DIVIDE_FLOAT, torricelly_inst_code::DIVIDE_DOUBLE);
      auto instruction = make_torricelly_instruction(instruction_code);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   case gasp::blaise::blaise_token_type::REMAINDER:
   {
      auto instruction = make_torricelly_instruction(torricelly_inst_code::REMINDER_INTEGER);
      torricelly_subroutine->append_instruction(instruction);
   }
   break;
   default:
      throw blaise_to_torricelly_error(expression->line(), expression->column(), "Unknown or unexpected binary operator.");
   }
}
