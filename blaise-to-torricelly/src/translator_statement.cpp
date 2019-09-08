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
      break;
   case ast::blaise_ast_statement_type::IF:
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
