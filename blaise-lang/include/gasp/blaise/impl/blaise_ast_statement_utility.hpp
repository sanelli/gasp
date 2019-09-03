#pragma once

#include <memory>

#include <gasp/blaise/impl/blaise_ast_statement.hpp>

namespace gasp::blaise::ast
{
class blaise_ast_statement_utility
{
public:
   static std::shared_ptr<blaise_ast_statement_compund> as_compound(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_assignment> as_assignment(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_subroutine_call> as_subroutine_call(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_if> as_if(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_for_loop> as_for_loop(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_dowhile_loop> as_do_while_loop(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_while_loop> as_while_loop(std::shared_ptr<blaise_ast_statement> statement);
   static std::shared_ptr<blaise_ast_statement_repeatuntil_loop> as_repeat_until_loop(std::shared_ptr<blaise_ast_statement> statement);
};

} // namespace gasp::blaise::ast
