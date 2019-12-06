#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <cmath>
#include <functional>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/common/internal_error.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

void blaise_to_torricelly::translator::translate_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                           std::map<std::string, unsigned int> &module_variables_mapping,
                                                           std::map<std::string, unsigned int> &variables_mapping,
                                                           std::shared_ptr<gasp::blaise::ast::blaise_ast_statement> statement,
                                                           unsigned int &max_stack_size) const
{

   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_statement" << std::endl);

   max_stack_size = 0U;
   switch (statement->type())
   {
   case ast::blaise_ast_statement_type::EMPTY:
   {
      auto empty_statement = ast::blaise_ast_statement_utility::as_empty(statement);
      translate_empty_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, empty_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::COMPOUND:
   {
      auto compound_statement = ast::blaise_ast_statement_utility::as_compound(statement);
      translate_compound_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, compound_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::ASSIGNEMENT:
   {
      auto assignment_statement = ast::blaise_ast_statement_utility::as_assignment(statement);
      translate_assignment_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, assignment_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::SUBROUTINE_CALL:
   {
      auto assignment_statement = ast::blaise_ast_statement_utility::as_subroutine_call(statement);
      translate_subroutine_call_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, assignment_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::IF:
   {
      auto if_statement = ast::blaise_ast_statement_utility::as_if(statement);
      translate_if_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, if_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::FOR_LOOP:
   {
      auto for_statement = ast::blaise_ast_statement_utility::as_for_loop(statement);
      translate_for_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, for_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::WHILE_LOOP:
   {
      auto whileloop_statement = ast::blaise_ast_statement_utility::as_while_loop(statement);
      translate_while_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, whileloop_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::DO_WHILE_LOOP:
   {
      auto dowhileloop_statement = ast::blaise_ast_statement_utility::as_do_while_loop(statement);
      translate_do_while_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, dowhileloop_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::REPEAT_UNTIL_LOOP:
   {
      auto repeatuntil_statement = ast::blaise_ast_statement_utility::as_repeat_until_loop(statement);
      translate_repeat_until_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, repeatuntil_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::DELETE:
   {
      auto delete_statement = ast::blaise_ast_statement_utility::as_delete(statement);
      translate_delete_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, delete_statement, max_stack_size);
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error(sanelli::make_string("Unknown statement type (ID = ", (int)statement->type(), ")"));
   }
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_empty_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                 std::map<std::string, unsigned int> &module_variables_mapping,
                                                                 std::map<std::string, unsigned int> &variables_mapping,
                                                                 std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_empty> statement,
                                                                 unsigned int &max_stack_size) const
{
   max_stack_size = 0U;
   auto noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
   torricelly_subroutine->append_instruction(noop);
}

void blaise_to_torricelly::translator::translate_compound_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                    std::map<std::string, unsigned int> &module_variables_mapping,
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
      translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, inner_statement, statement_max_stack_size);
      max_stack_size = std::max(max_stack_size, statement_max_stack_size);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_compound_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_assignment_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_assignment> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_assignment_statement" << std::endl);

   auto expression_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->expression(), expression_max_stack_size);

   max_stack_size = std::max(1U, expression_max_stack_size);

   std::string variable_name;
   auto identifier = statement->identifier();
   switch (identifier->type())
   {
   case blaise::ast::blaise_ast_identifier_type::VARIABLE:
   {
      auto variable_identifier = blaise::ast::blaise_ast_identifier_utility::as_variable_identifier(identifier);
      variable_name = variable_identifier->variable()->name();
      auto variable_index_it = variables_mapping.find(variable_name);
      if (variable_index_it == variables_mapping.end())
         throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Error while translating assignemt. Cannot find varibale '", variable_name, "'."));
      auto variable_index = variable_index_it->second;

      auto variable_type = variable_identifier->variable()->type();
      if (!ast::blaise_ast_utility::is_array(variable_type))
      {
         auto store_instruction_code = compute_instruction_code(statement->expression()->result_type(),
                                                                torricelly_inst_code::STORE_BYTE, torricelly_inst_code::STORE_SHORT, torricelly_inst_code::STORE_INTEGER, torricelly_inst_code::STORE_LONG,
                                                                torricelly_inst_code::STORE_FLOAT, torricelly_inst_code::STORE_DOUBLE,
                                                                torricelly_inst_code::STORE_CHAR, torricelly_inst_code::STORE_BOOLEAN);
         auto store_instruction = torricelly_instruction::make(store_instruction_code, variable_index, torricelly_inst_ref_type::SUBROUTINE);
         torricelly_subroutine->append_instruction(store_instruction);
      }
      else
      {
         auto array_type = ast::blaise_ast_utility::as_array_type(variable_type);
         auto underlying_array_type = array_type->underlying_type();

         auto statement_expression_type = statement->expression()->result_type();
         if (!ast::blaise_ast_utility::is_array(variable_type))
            throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Error while translating assignemt. Cannot assigned expression to array '", variable_name, "'."));
         // TODO: Check dimensions: right now it can only be just one

         auto expression_array_type = ast::blaise_ast_utility::as_array_type(statement_expression_type);
         auto expression_underlying_array_type = expression_array_type->underlying_type();

         if (!underlying_array_type->equals(expression_underlying_array_type))
            throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Error while translating assignemt. Array '", variable_name, "' has a different type of expression."));

         auto store_instruction = torricelly_instruction::make(torricelly_inst_code::STORE_ARRAY, variable_index, torricelly_inst_ref_type::SUBROUTINE);
         torricelly_subroutine->append_instruction(store_instruction);
      }
   }
   break;
   case blaise::ast::blaise_ast_identifier_type::ARRAY:
   {
      auto array_identifier = blaise::ast::blaise_ast_identifier_utility::as_array_identifier(identifier);
      variable_name = array_identifier->variable()->name();

      auto variable_index_it = variables_mapping.find(variable_name);
      if (variable_index_it == variables_mapping.end())
         throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Error while translating assignemt for array. Cannot find varibale '", variable_name, "'."));
      auto variable_index = variable_index_it->second;

      // Translate expression of the
      for(auto indexing = 0; indexing < array_identifier->count_indexes(); ++indexing){
         auto indexing_expression_max_stack_size = 0U;
         translate_expression(torricelly_module, torricelly_subroutine,
                              module_variables_mapping, variables_mapping,
                              array_identifier->indexing(indexing),
                              indexing_expression_max_stack_size);
         max_stack_size = std::max(max_stack_size, indexing_expression_max_stack_size);
      }

      // Push the size of the dimensions on the stack
      int32_t dimensions = array_identifier->count_indexes();
      auto size_value_variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(dimensions));
      auto load_size_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, size_value_variable_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(load_size_instruction);

      max_stack_size = std::max(max_stack_size, 2U);
      auto array_type = gasp::blaise::ast::blaise_ast_utility::as_array_type(array_identifier->variable()->type());
      auto store_instruction_code = compute_instruction_code(array_type->underlying_type(),
                                                             torricelly_inst_code::STORE_ARRAY_BYTE, torricelly_inst_code::STORE_ARRAY_SHORT, torricelly_inst_code::STORE_ARRAY_INTEGER, torricelly_inst_code::STORE_ARRAY_LONG,
                                                             torricelly_inst_code::STORE_ARRAY_FLOAT, torricelly_inst_code::STORE_ARRAY_DOUBLE,
                                                             torricelly_inst_code::STORE_ARRAY_CHAR, torricelly_inst_code::STORE_ARRAY_BOOLEAN);
      auto store_instruction = torricelly_instruction::make(store_instruction_code, variable_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(store_instruction);
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Unexpected identifier type when translating assignemt statement.");
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_assignment_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_if_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_if> statement, unsigned int &max_stack_size) const
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
   translate_condition(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->condition(), condition_max_stack_size);

   auto on_true_label = torricelly_subroutine->next_label();
   auto on_done_label = torricelly_subroutine->next_label();

   auto jump_eq_zero_instrution = torricelly_instruction::make(torricelly_inst_code::JUMP_EQ_ZERO, on_true_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_eq_zero_instrution);

   auto else_max_stack_size = 0U;
   if (statement->else_statement() != nullptr)
      translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->else_statement(), else_max_stack_size);

   auto jump_to_done = torricelly_instruction::make(torricelly_inst_code::JUMP, on_done_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_to_done);

   auto on_true_noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
   on_true_noop.set_label(on_true_label);
   torricelly_subroutine->append_instruction(on_true_noop);

   auto then_max_stack_size = 0U;
   translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->then_statement(), then_max_stack_size);

   auto on_done_noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
   on_done_noop.set_label(on_done_label);
   torricelly_subroutine->append_instruction(on_done_noop);

   max_stack_size = std::max({(1 + condition_max_stack_size), then_max_stack_size, else_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_if_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_subroutine_call_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_subroutine_call> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_subroutine_call_statement" << std::endl);

   auto subroutine = statement->subroutine();
   auto subroutine_max_stack_size = 0U;
   translate_subroutine_call(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping,
                             subroutine,
                             [statement](unsigned int index) { return statement->actual_parameter(index); },
                             subroutine_max_stack_size);

   auto returns_void = subroutine->return_type()->equals(blaise::ast::make_plain_type(blaise::ast::blaise_ast_system_type::VOID));

   max_stack_size = returns_void ? 0U : 1U;
   auto number_of_parameters = subroutine->count_parameters();
   max_stack_size = std::max(max_stack_size, number_of_parameters);

   // If not returning void we gotta pop up what's left on the stack;
   if (!returns_void)
   {
      auto pop_instruction_code = compute_instruction_code(subroutine->return_type(),
                                                           torricelly_inst_code::POP_BYTE, torricelly_inst_code::POP_SHORT, torricelly_inst_code::POP_INTEGER, torricelly_inst_code::POP_LONG,
                                                           torricelly_inst_code::POP_FLOAT, torricelly_inst_code::POP_DOUBLE,
                                                           torricelly_inst_code::POP_CHAR, torricelly_inst_code::POP_CHAR);

      auto pop_instruction = torricelly_instruction::make(pop_instruction_code);
      torricelly_subroutine->append_instruction(pop_instruction);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_subroutine_call_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_while_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_while_loop> statement, unsigned int &max_stack_size) const
{

   // [start]: NOOP
   // <condition>
   // LOAD_BOOLEAN [true]
   // CMP_BOOLEAN
   // JMP_NEQ_ZERO [on_done]
   // <body>
   // JUMP [start]
   // [on_done]: NOOP

   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_while_statement" << std::endl);

   auto on_start_label = torricelly_subroutine->next_label();
   auto on_done_label = torricelly_subroutine->next_label();

   auto start_noop_instruction = torricelly_instruction::make(torricelly_inst_code::NOOP);
   start_noop_instruction.set_label(on_start_label);
   torricelly_subroutine->append_instruction(start_noop_instruction);

   auto condition_max_stack_size = 0U;
   translate_condition(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->condition(), condition_max_stack_size);

   auto jump_neq_zero_instrution = torricelly_instruction::make(torricelly_inst_code::JUMP_NOT_ZERO, on_done_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_neq_zero_instrution);

   auto body_max_stack_size = 0U;
   translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->body(), body_max_stack_size);

   auto jump_to_start_instruction = torricelly_instruction::make(torricelly_inst_code::JUMP, on_start_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_to_start_instruction);

   auto done_noop_instruction = torricelly_instruction::make(torricelly_inst_code::NOOP);
   done_noop_instruction.set_label(on_done_label);
   torricelly_subroutine->append_instruction(done_noop_instruction);

   max_stack_size = std::max({1U, condition_max_stack_size, body_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_while_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_do_while_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_dowhile_loop> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_do_while_statement" << std::endl);

   // [start]: NOOP
   // <body>
   // <condition>
   // LOAD_BOOLEAN [true]
   // CMP_BOOLEAN
   // JMP_EQ_ZERO [start]

   auto on_start_label = torricelly_subroutine->next_label();

   auto start_noop_instruction = torricelly_instruction::make(torricelly_inst_code::NOOP);
   start_noop_instruction.set_label(on_start_label);
   torricelly_subroutine->append_instruction(start_noop_instruction);

   auto body_max_stack_size = 0U;
   translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->body(), body_max_stack_size);

   auto condition_max_stack_size = 0U;
   translate_condition(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->condition(), condition_max_stack_size);

   auto jump_eq_zero_instrution = torricelly_instruction::make(torricelly_inst_code::JUMP_EQ_ZERO, on_start_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_eq_zero_instrution);

   max_stack_size = std::max({1U, condition_max_stack_size, body_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_do_while_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_repeat_until_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_repeatuntil_loop> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_repeat_until_statement" << std::endl);

   // [start]: NOOP
   // <body>
   // <condition>
   // LOAD_BOOLEAN [true]
   // CMP_BOOLEAN
   // JMP_NOT_ZERO [start]

   auto on_start_label = torricelly_subroutine->next_label();

   auto start_noop_instruction = torricelly_instruction::make(torricelly_inst_code::NOOP);
   start_noop_instruction.set_label(on_start_label);
   torricelly_subroutine->append_instruction(start_noop_instruction);

   auto body_max_stack_size = 0U;
   translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->body(), body_max_stack_size);

   auto condition_max_stack_size = 0U;
   translate_condition(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->condition(), condition_max_stack_size);

   auto jump_neq_zero_instrution = torricelly_instruction::make(torricelly_inst_code::JUMP_NOT_ZERO, on_start_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_neq_zero_instrution);

   max_stack_size = std::max({1U, condition_max_stack_size, body_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_repeat_until_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_for_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_for_loop> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_for_statement" << std::endl);

   // <from_expression>
   // STORE_INTEGER [index_variable]
   // <to_expression>
   // STORE_INTEGER [upper_limit_variable]
   // <step_expression>
   // STORE_INTEGER [step_variable]
   // [begin] NOOP
   // LOAD_INTEGER [index_variable]
   // LOAD_INTEGER [upper_limit_variable]
   // CMP_INTEGER
   // JUMP_GT [done]
   // <body>
   // LOAD_INTEGER [step_variable]
   // LOAD_INTEGER [index_variable]
   // ADD_INTEGER
   // STORE_INTEGER [index_variable]
   // JUMP [begin]
   // [done] NOOP

   // Get the loop variable index
   auto variable_identifier = blaise::ast::blaise_ast_identifier_utility::as_variable_identifier(statement->variable())->variable();
   auto variable_index_index_it = variables_mapping.find(variable_identifier->name());
   if (variable_index_index_it == variables_mapping.end())
      throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Cannot find variable '", variable_identifier->name(), "'."));
   auto variable_indexer_index = variable_index_index_it->second;

   // create the required temporaries
   auto upper_limit_variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(0));
   auto step_variable_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(1));

   // get all the required labels
   auto start_label = torricelly_subroutine->next_label();
   auto done_label = torricelly_subroutine->next_label();

   // Initialize all variables involved
   auto from_expression_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->from_expression(), from_expression_max_stack_size);
   auto initialize_indexer_inst = torricelly_instruction::make(torricelly_inst_code::STORE_INTEGER, variable_indexer_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(initialize_indexer_inst);

   auto upper_limit_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->to_expression(), upper_limit_max_stack_size);
   auto initialize_upper_limit_inst = torricelly_instruction::make(torricelly_inst_code::STORE_INTEGER, upper_limit_variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(initialize_upper_limit_inst);

   auto step_expression_max_stack_size = 0U;
   if (statement->step_expression() != nullptr)
   {
      translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->step_expression(), step_expression_max_stack_size);
      auto initialize_upper_limit_inst = torricelly_instruction::make(torricelly_inst_code::STORE_INTEGER, step_variable_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(initialize_upper_limit_inst);
   }

   // Add loop header
   auto loop_header_inst = torricelly_instruction::make(torricelly_inst_code::NOOP);
   loop_header_inst.set_label(start_label);
   torricelly_subroutine->append_instruction(loop_header_inst);

   // Perform check of boundaries
   auto load_indexer_inst = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, variable_indexer_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_indexer_inst);
   auto load_upper_limit_inst = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, upper_limit_variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_upper_limit_inst);
   auto compare_inst = torricelly_instruction::make(torricelly_inst_code::CMP_INTEGER);
   torricelly_subroutine->append_instruction(compare_inst);
   auto jump_condition_inst = torricelly_instruction::make(torricelly_inst_code::JUMP_GT_ZERO, done_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_condition_inst);

   // execute the body
   auto body_max_stack_size = 0U;
   translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement->body(), body_max_stack_size);

   // Increment the indexer
   load_indexer_inst = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, variable_indexer_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_indexer_inst);
   auto load_step_inst = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, step_variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_step_inst);
   auto add_inst = torricelly_instruction::make(torricelly_inst_code::ADD_INTEGER);
   torricelly_subroutine->append_instruction(add_inst);
   auto store_indexer_inst = torricelly_instruction::make(torricelly_inst_code::STORE_INTEGER, variable_indexer_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(store_indexer_inst);

   // Jump to header
   auto jump_to_header_inst = torricelly_instruction::make(torricelly_inst_code::JUMP, start_label, torricelly_inst_ref_type::LABEL);
   torricelly_subroutine->append_instruction(jump_to_header_inst);

   // done
   auto done_inst = torricelly_instruction::make(torricelly_inst_code::NOOP);
   done_inst.set_label(done_label);
   torricelly_subroutine->append_instruction(done_inst);

   // COmpute max stack size from everything that happened in the loop
   max_stack_size = std::max({2U, from_expression_max_stack_size, upper_limit_max_stack_size, step_expression_max_stack_size, body_max_stack_size}, std::less<unsigned int>());

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_for_statement" << std::endl);
}

void blaise_to_torricelly::translator::translate_delete_statement(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping, std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_delete> statement, unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_new_statement" << std::endl);

   // Get the variable
   auto variable_identifier = blaise::ast::blaise_ast_identifier_utility::as_variable_identifier(statement->identifier())->variable();
   auto varaiable_iterator = variables_mapping.find(variable_identifier->name());
   if (varaiable_iterator == variables_mapping.end())
      throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Cannot find variable '", variable_identifier->name(), "'."));
   auto variable_index = varaiable_iterator->second;

   // Check the variable is an array
   if (!ast::blaise_ast_utility::is_array(variable_identifier->type()))
      throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Error while translating delete statemente. Variable '", variable_identifier->name(), "' must be of array type."));

   // Check the array is with undefined dimensions
   auto array_type = ast::blaise_ast_utility::as_array_type(variable_identifier->type());
   if (!array_type->is_unbounded())
      throw blaise_to_torricelly_error(statement->line(), statement->column(), sanelli::make_string("Error while translating delete statemente. Variable '", variable_identifier->name(), "' must be an unbounded array type."));

   // Create the delete instrution
   auto instruction = torricelly_instruction::make(torricelly_inst_code::FREE_ARRAY, variable_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(instruction);

   max_stack_size = 0U;

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_new_statement" << std::endl);
}
