#include <memory>
#include <vector>
#include <string>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>
#include <gasp/common/output.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/ast.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_statement(blaise_parser_context &context,
            std::shared_ptr<ast::blaise_ast_statement_compund> parent)
{
   std::shared_ptr<ast::blaise_ast_statement> statement = nullptr;

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_statement" << std::endl);

   const auto token = context.peek_token();
   const auto token_type = token.type();

   switch (token_type)
   {
   case blaise_token_type::IDENTIFIER:
   {
      const auto lookahead = context.peek_token(1);
      const auto lookahead_type = lookahead.type();

      switch (lookahead_type)
      {
      case blaise_token_type::LEFT_PARENTHESES:
         statement = parse_subroutine_call_statement(context);
         break;
      case blaise_token_type::ASSIGNMENT:
         statement = parse_assignamet_statement(context);
         break;
      default:
         throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found after indetifier."));
      }
   }
   break;
   case blaise_token_type::BEGIN:
      statement = parse_compound_statement(context, parent);
      break;
      // TODO: Add support for other kind of statement
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found."));
   }
   match_token(context, blaise_token_type::SEMICOLON);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_statement" << std::endl);

   return statement;
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_compound_statement(blaise_parser_context &context, std::shared_ptr<ast::blaise_ast_statement_compund> parent)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_compound_statement" << std::endl);

   match_token(context, blaise_token_type::BEGIN);
   auto compund_statement = ast::make_compound_statement();
   if(parent != nullptr)
      parent->push_back(compund_statement);
   while (!is_token(context, blaise_token_type::END))
   {
      auto statement = parse_statement(context);
      compund_statement->push_back(statement);
   }
   match_token(context, blaise_token_type::END);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_compound_statement" << std::endl);
   return compund_statement;
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_subroutine_call_statement(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_call_statement" << std::endl);

   auto identifier_token = context.peek_token();
   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   vector<shared_ptr<ast::blaise_ast_expression>> expressions;
   vector<ast::blaise_ast_type> types;
   parse_subroutine_call_parameters(context, expressions, types);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   auto subroutine = context.module()->get_subroutine(identifier_token, types);
   // TODO: Lookup in referenced modules
   if(subroutine == nullptr) {
      throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(), make_string("Cannot find function '", identifier_token.value(),"(", types ,")'"));
   }

   auto statement = ast::make_blaise_ast_statement_subroutine_call(subroutine, expressions);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_subroutine_call_statement" << std::endl);

   return statement;
}

void blaise_parser::parse_subroutine_call_parameters(blaise_parser_context &context, 
      std::vector<std::shared_ptr<ast::blaise_ast_expression>>& expressions,
      std::vector<ast::blaise_ast_type>& types
      )
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_call_parameters" << std::endl);

   auto token_type = context.peek_token().type();

   if (token_type != blaise_token_type::RIGHT_PARENTHESES)
      do
      {
         auto expression = parse_expression(context);
         expressions.push_back(expression);
         types.push_back(expression->result_type());
         
         if (is_token_and_match(context, blaise_token_type::COMMA))
            continue;
         else if (!is_token(context, blaise_token_type::RIGHT_PARENTHESES))
         {
            auto token = context.peek_token();
            throw_parse_error_with_details(context, token.line(), token.column(), "expected ')' or ','.");
         }
         token_type = context.peek_token().type();
      } while (token_type != blaise_token_type::RIGHT_PARENTHESES);

    GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_subroutine_call_parameters" << std::endl);

}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_assignamet_statement(blaise_parser_context &context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_assignment" << std::endl);

   auto identifier = context.peek_token();
   match_token(context, blaise_token_type::IDENTIFIER);
   auto variable = context.current_subroutine()->get_variable(identifier.value());

   if(variable == nullptr)
         throw_parse_error_with_details(context, identifier.line(), identifier.column(), make_string("Variable ", identifier.value(), " does not exists in the context."));

   match_token(context, blaise_token_type::ASSIGNMENT);
   auto expression = parse_expression(context);

   auto statement = ast::make_assignement_statement(identifier.line(), identifier.column(), variable, expression);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_assignment" << std::endl);

   return statement;
}

