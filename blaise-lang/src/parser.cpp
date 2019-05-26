#include <vector>
#include <algorithm>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

void blaise_parser::parse(blaise_parser_context &context) const
{
   unsigned int index = 0;
   if (is_token(context, blaise_token::PROGRAM))
      parse_program(context);
   else
      throw_parse_error_with_details(context, "only programs are supported.");

   // If more tokens are available at this stage then the code is malformed
   if (context.has_more_tokens())
      throw_parse_error_with_details(context, "too many tokens at the end of the program");
}

void blaise_parser::parse_program(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_program" << std::endl);

   match_token(context, blaise_token::PROGRAM);
   auto program_name = match_token(context, blaise_token::IDENTIFIER);
   match_token(context, blaise_token::SEMICOLON);

   // TODO: match uses
   // TODO: match functions and procedures
   // TODO: match constants

   parse_variables_declaration(context);
   parse_compound_statement(context);

   match_token(context, blaise_token::PERIOD);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_program" << std::endl);
}

void blaise_parser::parse_variables_declaration(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variables_declaration" << std::endl);

   if (!is_token(context, blaise_token::VAR))
      return; // No such variables
   match_token(context, blaise_token::VAR);
   while (is_token(context, blaise_token::IDENTIFIER) &&
          (is_token(context, blaise_token::COLON, 1) || is_token(context, blaise_token::COMMA, 1)))
   {
      parse_variable_declaration(context);
   }

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variables_declaration" << std::endl);
}

void blaise_parser::parse_variable_declaration(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_declaration" << std::endl);

   std::vector<std::string> variable_names;
   parse_variable_names_list(context, variable_names);
   match_token(context, blaise_token::COLON);
   parse_variable_type(context);
   match_token(context, blaise_token::SEMICOLON);
   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_declaration" << std::endl);
}

void blaise_parser::parse_variable_names_list(blaise_parser_context &context, std::vector<std::string> &variable_names)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_names_list" << std::endl);

   // TODO: Possible generate a new variable definition, not just the name
   do
   {
      variable_names.push_back(match_token(context, blaise_token::IDENTIFIER));
   } while (is_token_and_match(context, blaise_token::COMMA));

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_names_list" << std::endl);
}

void blaise_parser::parse_variable_type(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_type" << std::endl);

   const auto is_unsigned = is_token_and_match(context, blaise_token::UNSIGNED);
   const auto token = context.peek_token();
   const auto token_type = token.type();
   if (is_unsigned && !blaise_token_utility::is_unsigned_type(token_type))
      throw parser_error("unexpected type after UNSIGNED keyword");
   else if (!blaise_token_utility::is_type(token_type))
      throw parser_error(token.line(), token.column(), "unexpected type");
   match_token(context, token_type);

   // TODO: Return the type definition
   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_type" << std::endl);
}

void blaise_parser::parse_statement(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_statement" << std::endl);

   const auto token = context.peek_token();
   const auto token_type = token.type();

   switch (token_type)
   {
   case blaise_token::IDENTIFIER:
   {
      const auto lookahead = context.peek_token(1);
      const auto lookahead_type = lookahead.type();

      switch (lookahead_type)
      {
      case blaise_token::LEFT_PARENTHESES:
         parse_function_call_statement(context);
         break;
      case blaise_token::ASSIGNMENT:
         parse_assignamet_statement(context);
         break;
      default:
         throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found after indetifier."));
      }
   }
   break;
   case blaise_token::BEGIN:
      parse_compound_statement(context);
      break;
      // TODO: Add support for other kind of statement
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found."));
   }
   match_token(context, blaise_token::SEMICOLON);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_statement" << std::endl);
}

void blaise_parser::parse_compound_statement(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_compound_statement" << std::endl);

   match_token(context, blaise_token::BEGIN);
   while (!is_token(context, blaise_token::END))
   {
      parse_statement(context);
   }
   match_token(context, blaise_token::END);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_compound_statement" << std::endl);
}

void blaise_parser::parse_function_call_statement(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_function_call_statement" << std::endl);

   auto function_name = match_token(context, blaise_token::IDENTIFIER);
   match_token(context, blaise_token::LEFT_PARENTHESES);
   parse_function_call_parameters(context);
   match_token(context, blaise_token::RIGHT_PARENTHESES);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_function_call_statement" << std::endl);
}

void blaise_parser::parse_function_call_parameters(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_function_call_parameters" << std::endl);

   auto token_type = context.peek_token().type();

   if (token_type != blaise_token::RIGHT_PARENTHESES)
      do
      {
         parse_expression(context);

         const auto token = context.peek_token();
         token_type = token.type();
         if (is_token_and_match(context, blaise_token::COMMA))
            continue;
         else if (!is_token(context, blaise_token::RIGHT_PARENTHESES))
            throw_parse_error_with_details(context, token.line(), token.column(), "expected ')' or ','.");

      } while (token_type != blaise_token::RIGHT_PARENTHESES);

    GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_function_call_parameters" << std::endl);

}

void blaise_parser::parse_assignamet_statement(blaise_parser_context &context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_assignment" << std::endl);
   auto identifier = match_token(context, blaise_token::IDENTIFIER);
   match_token(context, blaise_token::ASSIGNMENT);
   parse_expression(context);
   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_assignment" << std::endl);
}

void blaise_parser::parse_expression(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression" << std::endl);

   const auto token = context.peek_token();
   const auto token_type = token.type();

   switch (token_type)
   {
   case blaise_token::IDENTIFIER:
      // TODO: What to do with the identifier?
      {
         const auto identifier = match_token(context, blaise_token::IDENTIFIER);
      }
      break;
      // TODO: Add support for all other kind of expression
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found."));
   }

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression" << std::endl);

}
