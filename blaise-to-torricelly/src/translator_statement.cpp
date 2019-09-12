#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <cmath>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/common/internal_error.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

void blaise_to_torricelly::translator::translate_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                           const std::map<std::string, unsigned int> &module_variables_mapping,
                                                           std::map<std::string, unsigned int> &variables_mapping,
                                                           std::shared_ptr<gasp::blaise::ast::blaise_ast_statement> statement,
                                                           unsigned int &max_stack_size) const
{

   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_statement" << std::endl);

   max_stack_size = 0U;
   switch (statement->type())
   {
   case ast::blaise_ast_statement_type::COMPOUND:
   {
      auto compound_statement = ast::blaise_ast_statement_utility::as_compound(statement);
      translate_compound_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, compound_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::ASSIGNEMENT:
   {
      auto assignment_statement = ast::blaise_ast_statement_utility::as_assignment(statement);
      translate_assignment_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, assignment_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::SUBROUTINE_CALL:
   {
      auto assignment_statement = ast::blaise_ast_statement_utility::as_subroutine_call(statement);
      translate_subroutine_call_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, assignment_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::IF:
   {
      auto if_statement = ast::blaise_ast_statement_utility::as_if(statement);
      translate_if_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, if_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::FOR_LOOP:
      break;
   case ast::blaise_ast_statement_type::DO_WHILE_LOOP:
      break;
   case ast::blaise_ast_statement_type::WHILE_LOOP:
      break;
   case ast::blaise_ast_statement_type::REPEAT_UNTIL_LOOP:
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unknown statement type");
   }
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_compound_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                    const std::map<std::string, unsigned int> &module_variables_mapping,
                                                                    std::map<std::string, unsigned int> &variables_mapping,
                                                                    std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_compund> statement,
                                                                    unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_compound_statement" << std::endl);

   max_stack_size = 0U;
   auto statement_count = statement->get_statements_count();
   for (auto index = 0; index < statement_count; ++index)
   {
      auto inner_statement = statement->get_statement(index);
      auto statement_max_stack_size = 0U;
      translate_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, inner_statement, statement_max_stack_size);
      max_stack_size = std::max(max_stack_size, statement_max_stack_size);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_compound_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_assignment_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, const std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_assignment> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_assignment_statement" << std::endl);

   auto expression_max_stack_size = 0U;
   translate_expression(torricelly_subroutine, module_variables_mapping, variables_mapping, statement->expression(), expression_max_stack_size);

   max_stack_size = std::max(1U, expression_max_stack_size);

   std::string variable_name;
   auto identifier = statement->identifier();
   switch (identifier->type())
   {
   case blaise::ast::blaise_ast_identifier_type::VARIABLE:
   {
      auto variable_identifier = blaise::ast::blaise_ast_identifier_utility::as_variable_identifier(identifier);
      variable_name = variable_identifier->variable()->name();
   }
   break;
   case blaise::ast::blaise_ast_identifier_type::ARRAY:
      throw blaise_to_torricelly_internal_error("ARRAY identifier type unsupported when translating assignemt statement.");
      break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected identifier type when translating assignemt statement.");
   }

   auto variable_index_it = variables_mapping.find(variable_name);
   if (variable_index_it == variables_mapping.end())
      throw blaise_to_torricelly_internal_error(sanelli::make_string("Error while translating assignemt. Cannot find varibale '", variable_name, "'."));
   auto variable_index = variable_index_it->second;

   auto store_instruction_code = compute_instruction_code(statement->expression()->result_type(),
                                                          torricelly_inst_code::STORE_INTEGER,
                                                          torricelly_inst_code::STORE_FLOAT, torricelly_inst_code::STORE_DOUBLE,
                                                          torricelly_inst_code::STORE_CHAR, torricelly_inst_code::STORE_BOOLEAN);
   auto store_instruction = make_torricelly_instruction(store_instruction_code, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(store_instruction);

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_assignment_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_if_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, const std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_if> statement, unsigned int &max_stack_size) const
{
   // <condition>
   // LOAD_BOOLEAN [true]
   // CMP_BOOLEAN
   // JMP_EQ_ZERO [on_true]
   // <else_statement>
   // JUMP [on_done]
   // [on_true]: NOOP
   // <then_statement>
   // [on_done]: NOOP

   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_if_statement" << std::endl);

   auto condition_max_stack_size = 0U;
   translate_condition(torricelly_subroutine, module_variables_mapping, variables_mapping, statement->condition(), condition_max_stack_size);

   auto on_true_label = torricelly_subroutine->next_label();
   auto on_done_label = torricelly_subroutine->next_label();

   auto jump_eq_zero_instrution = make_torricelly_instruction(torricelly_inst_code::JUMP_EQ_ZERO, on_true_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_eq_zero_instrution);

   auto else_max_stack_size = 0U;
   translate_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, statement->else_statement(), else_max_stack_size);

   auto jump_to_done = make_torricelly_instruction(torricelly_inst_code::JUMP, on_done_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_to_done);

   auto on_true_noop = make_torricelly_instruction(torricelly_inst_code::NOOP);
   on_true_noop->set_label(on_true_label);
   torricelly_subroutine->append_instruction(on_true_noop);

   auto then_max_stack_size = 0U;
   translate_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, statement->then_statement(), then_max_stack_size);

   auto on_done_noop = make_torricelly_instruction(torricelly_inst_code::NOOP);
   on_done_noop->set_label(on_done_label);
   torricelly_subroutine->append_instruction(on_done_noop);

   max_stack_size = std::max({(1 + condition_max_stack_size), then_max_stack_size, else_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_if_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_subroutine_call_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, const std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_subroutine_call> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_subroutine_call_statement" << std::endl);

  auto subroutine = statement->subroutine();
  auto subroutine_max_stack_size = 0U;
  translate_subroutine_call(torricelly_subroutine, module_variables_mapping, variables_mapping,
      subroutine,
      [statement](unsigned int index) { return statement->actual_parameter(index); },
      subroutine_max_stack_size);

   auto returns_void = subroutine->return_type()->equals(blaise::ast::make_plain_type(blaise::ast::blaise_ast_system_type::VOID));

   max_stack_size = returns_void ? 0U : 1U;
   auto number_of_parameters = subroutine->count_parameters();
   max_stack_size = std::max(max_stack_size, number_of_parameters);

   // If not returning void we gotta pop up what's left on the stack;
   if(!returns_void) { 
      auto pop_instruction_code = compute_instruction_code(subroutine->return_type(), torricelly_inst_code::POP_INTEGER,
                                                                  torricelly_inst_code::POP_FLOAT, torricelly_inst_code::POP_DOUBLE, 
                                                                  torricelly_inst_code::POP_CHAR, torricelly_inst_code::POP_CHAR);

      auto pop_instruction = make_torricelly_instruction(pop_instruction_code);
      torricelly_subroutine->append_instruction(pop_instruction);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_subroutine_call_statement" << std::endl);
}