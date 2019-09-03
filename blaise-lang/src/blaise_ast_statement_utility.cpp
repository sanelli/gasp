#include <vector>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/ast.hpp>
#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace std;

using namespace gasp::blaise;
using namespace gasp::blaise::ast;

std::shared_ptr<blaise_ast_statement_compund> blaise_ast_statement_utility::as_compound(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::COMPOUND)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into assignment statement");

   return std::static_pointer_cast<blaise_ast_statement_compund>(statement);
}

std::shared_ptr<blaise_ast_statement_assignment> blaise_ast_statement_utility::as_assignment(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::ASSIGNEMENT)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into assignment statement");

   return std::static_pointer_cast<blaise_ast_statement_assignment>(statement);
}
std::shared_ptr<blaise_ast_statement_subroutine_call> blaise_ast_statement_utility::as_subroutine_call(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::SUBROUTINE_CALL)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into subroutine call statement");

   return std::static_pointer_cast<blaise_ast_statement_subroutine_call>(statement);
}
std::shared_ptr<blaise_ast_statement_if> blaise_ast_statement_utility::as_if(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::IF)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into if statement");

   return std::static_pointer_cast<blaise_ast_statement_if>(statement);
}
std::shared_ptr<blaise_ast_statement_for_loop> blaise_ast_statement_utility::blaise_ast_statement_utility::as_for_loop(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::FOR_LOOP)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into for loop statement");

   return std::static_pointer_cast<blaise_ast_statement_for_loop>(statement);
}
std::shared_ptr<blaise_ast_statement_dowhile_loop> as_do_while_loop(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::DO_WHILE_LOOP)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into do-while loop statement");

   return std::static_pointer_cast<blaise_ast_statement_dowhile_loop>(statement);
}
std::shared_ptr<blaise_ast_statement_while_loop> blaise_ast_statement_utility::as_while_loop(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::WHILE_LOOP)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into while loop statement");

   return std::static_pointer_cast<blaise_ast_statement_while_loop>(statement);
}
std::shared_ptr<blaise_ast_statement_repeatuntil_loop> blaise_ast_statement_utility::as_repeat_until_loop(std::shared_ptr<blaise_ast_statement> statement)
{
   if (statement->type() != blaise_ast_statement_type::REPEAT_UNTIL_LOOP)
      throw blaise_ast_error(statement->line(), statement->column(), "Cannot convert statetemt into repeat-until loop statement");

   return std::static_pointer_cast<blaise_ast_statement_repeatuntil_loop>(statement);
}
