#include <memory>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/language.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

void blaise_parser::parse_statement(blaise_parser_context &context)
{
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
         parse_function_call_statement(context);
         break;
      case blaise_token_type::ASSIGNMENT:
         parse_assignamet_statement(context);
         break;
      default:
         throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found after indetifier."));
      }
   }
   break;
   case blaise_token_type::BEGIN:
      parse_compound_statement(context);
      break;
      // TODO: Add support for other kind of statement
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found."));
   }
   match_token(context, blaise_token_type::SEMICOLON);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_statement" << std::endl);
}

void blaise_parser::parse_compound_statement(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_compound_statement" << std::endl);

   match_token(context, blaise_token_type::BEGIN);
   while (!is_token(context, blaise_token_type::END))
   {
      parse_statement(context);
   }
   match_token(context, blaise_token_type::END);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_compound_statement" << std::endl);
}

void blaise_parser::parse_function_call_statement(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_function_call_statement" << std::endl);

   auto function_name = match_token_and_get_value(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   parse_function_call_parameters(context);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_function_call_statement" << std::endl);
}

void blaise_parser::parse_function_call_parameters(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_function_call_parameters" << std::endl);

   auto token_type = context.peek_token().type();

   if (token_type != blaise_token_type::RIGHT_PARENTHESES)
      do
      {
         parse_expression(context);

         const auto token = context.peek_token();
         token_type = token.type();
         if (is_token_and_match(context, blaise_token_type::COMMA))
            continue;
         else if (!is_token(context, blaise_token_type::RIGHT_PARENTHESES))
            throw_parse_error_with_details(context, token.line(), token.column(), "expected ')' or ','.");

      } while (token_type != blaise_token_type::RIGHT_PARENTHESES);

    GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_function_call_parameters" << std::endl);

}

void blaise_parser::parse_assignamet_statement(blaise_parser_context &context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_assignment" << std::endl);
   auto identifier = match_token_and_get_value(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::ASSIGNMENT);
   parse_expression(context);
   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_assignment" << std::endl);
}

