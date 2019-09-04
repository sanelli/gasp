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

void blaise_to_torricelly::translator::translate_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                           std::map<std::string, unsigned int> &variables_mapping,
                                                           std::shared_ptr<gasp::blaise::ast::blaise_ast_statement> statement,
                                                           unsigned int &max_stack_size) const
{
   max_stack_size = 0U;
   switch (statement->type())
   {
   case ast::blaise_ast_statement_type::COMPOUND:
   {
      auto compound_statement = ast::blaise_ast_statement_utility::as_compound(statement);
      translate_compound_statement(torricelly_subroutine, variables_mapping, compound_statement, max_stack_size);
   }
   break;
   case ast::blaise_ast_statement_type::ASSIGNEMENT:
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
}

void blaise_to_torricelly::translator::translate_compound_statement(std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                    std::map<std::string, unsigned int> &variables_mapping,
                                                                    std::shared_ptr<gasp::blaise::ast::blaise_ast_statement_compund> statement,
                                                                    unsigned int &max_stack_size) const
{
}
