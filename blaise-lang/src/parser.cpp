#include <vector>
#include <algorithm>

#include <gasp/common/tokenizer.hpp>

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
      throw parser_error("only programs are supported.");

   // If more tokens are available at this stage then the code is malformed
   if (context.has_more_tokens())
      throw parser_error("too many tokens at the end of the program");
}

void blaise_parser::parse_program(blaise_parser_context &context)
{
   match_token(context, blaise_token::PROGRAM);
   auto program_name = match_token(context, blaise_token::IDENTIFIER);
   match_token(context, blaise_token::SEMICOLON);

   // TODO: match uses
   // TODO: match functions and procedures
   // TODO: match constants
   parse_variables_declaration(context);

   match_token(context, blaise_token::BEGIN);

   while (!is_token(context, blaise_token::END))
   {
      // TODO: match statements
   }
   match_token(context, blaise_token::END);
   match_token(context, blaise_token::PERIOD);
}

void blaise_parser::parse_variables_declaration(blaise_parser_context &context)
{
   if (!is_token(context, blaise_token::VAR))
      return; // No such variables
   match_token(context, blaise_token::VAR);
   while (is_token(context, blaise_token::IDENTIFIER) && is_token(context, blaise_token::COLON, 1))
   {
      parse_variable_declaration(context);
   }
}

void blaise_parser::parse_variable_declaration(blaise_parser_context &context)
{
   std::vector<std::string> variable_names;
   match_token(context, blaise_token::COLON);
   parse_variable_type(context);
   match_token(context, blaise_token::SEMICOLON);
}

void blaise_parser::parse_variable_names_list(blaise_parser_context &context, std::vector<std::string> &variable_names)
{
   // TODO: Possible generate a new variable definition, not just the name
   do
   {
      variable_names.push_back(match_token(context, blaise_token::IDENTIFIER));
   } while (is_token_and_match(context, blaise_token::COMMA));
}

void blaise_parser::parse_variable_type(blaise_parser_context &context)
{
   bool is_unsigned = is_token_and_match(context, blaise_token::UNSIGNED);
   auto token = context.peek_token();
   auto token_type = token.type();
   if (is_unsigned && !blaise_token_utility::is_unsigned_type(token_type))
      throw parser_error("unexpected type after UNSIGNED keyword");
   else if (!blaise_token_utility::is_type(token_type))
      throw parser_error(token.line(), token.column(), "unexpected type");
   match_token(context, token_type);

   // TODO: Return the type definition
}
