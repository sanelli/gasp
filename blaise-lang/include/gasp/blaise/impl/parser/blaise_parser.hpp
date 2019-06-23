#pragma once

#include <memory>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/parser.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/ast.hpp>

#include <gasp/blaise/impl/parser/blaise_parser_context.hpp>

namespace gasp::blaise
{

class blaise_parser : public gasp::common::parser<blaise_token_type>
{
   // TODO: Create a match identifier so that in the future I can 
   //       handle identifiers followed by array definition

   static void parse_program(blaise_parser_context &context);
   static void parse_subroutines_declaration(blaise_parser_context &context);
   static void parse_function_declaration(blaise_parser_context &context);
   static void parse_procedure_declaration(blaise_parser_context &context);
   static void inline parse_subroutine_declaration_impl(blaise_parser_context &context, 
                              blaise_token_type expected_token_type, 
                              const char* caller_name);
   static void parse_subroutine_parameters(blaise_parser_context &context, 
                              std::vector<std::shared_ptr<ast::blaise_ast_type>>& param_types);
   static void parse_variables_declaration(blaise_parser_context &context);
   static void parse_variable_declaration(blaise_parser_context &context);
   static void parse_variable_names_list(blaise_parser_context &context, 
                              std::vector<gasp::common::token<blaise_token_type>> &variable_names);
   static std::shared_ptr<ast::blaise_ast_type> parse_variable_type(blaise_parser_context &context, bool accept_unbounded_array);

   // Statement
   static std::shared_ptr<ast::blaise_ast_statement> parse_statement(blaise_parser_context &context,
            std::shared_ptr<ast::blaise_ast_statement_compund> parent = nullptr);
   static std::shared_ptr<ast::blaise_ast_statement> parse_compound_statement(blaise_parser_context &context,
            std::shared_ptr<ast::blaise_ast_statement_compund> parent = nullptr);
   static std::shared_ptr<ast::blaise_ast_statement> parse_subroutine_call_statement(blaise_parser_context &context);
   static void parse_subroutine_call_parameters(blaise_parser_context &context, 
                              std::vector<std::shared_ptr<ast::blaise_ast_expression>>& expressions,
                              std::vector<std::shared_ptr<ast::blaise_ast_type>>& types);
   static std::shared_ptr<ast::blaise_ast_statement> parse_assignamet_statement(blaise_parser_context &context);
   static std::shared_ptr<ast::blaise_ast_statement> parse_if_statement(blaise_parser_context &context);

   // Expression
   static std::shared_ptr<ast::blaise_ast_expression> parse_expression(blaise_parser_context &context);
   static std::shared_ptr<ast::blaise_ast_expression> parse_expression_helper(blaise_parser_context &context, 
                              std::shared_ptr<ast::blaise_ast_expression> lhs, 
                              unsigned int min_precedence);
   static std::shared_ptr<ast::blaise_ast_expression> parse_expression_term(blaise_parser_context &context);
   static std::shared_ptr<ast::blaise_ast_expression> parse_number(blaise_parser_context& context);
   static std::shared_ptr<ast::blaise_ast_expression> parse_boolean(blaise_parser_context& context);
   static std::shared_ptr<ast::blaise_ast_expression> parse_cast_expression(blaise_parser_context& context);
   static std::shared_ptr<ast::blaise_ast_expression> parse_ternary_expression(blaise_parser_context& context);
   
public:
   void parse(blaise_parser_context &context) const;
};

} // namespace gasp::blaise