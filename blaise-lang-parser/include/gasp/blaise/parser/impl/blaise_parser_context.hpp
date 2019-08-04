#pragma once

#include <vector>
#include <memory>



#include <gasp/blaise/tokenizer/tokens.hpp>
#include <gasp/blaise/ast.hpp>

namespace gasp::blaise
{

class blaise_parser_context : public sanelli::parser_context<blaise_token_type>
{
   std::shared_ptr<ast::blaise_ast_module> _module;
   std::shared_ptr<ast::blaise_ast_subroutine> _main_subroutine;
   std::shared_ptr<ast::blaise_ast_subroutine> _current_subroutine;
public:
   blaise_parser_context();
   void module(std::shared_ptr<ast::blaise_ast_module> module);
   std::shared_ptr<ast::blaise_ast_module> module() const;
   void main_subroutine(std::shared_ptr<ast::blaise_ast_subroutine> subroutine);
   std::shared_ptr<ast::blaise_ast_subroutine> main_subroutine() const;
   void current_subroutine(std::shared_ptr<ast::blaise_ast_subroutine> subroutine);
   std::shared_ptr<ast::blaise_ast_subroutine> current_subroutine() const;

};

} // namespace gasp::blaise