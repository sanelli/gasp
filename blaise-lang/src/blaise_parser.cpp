#include <vector>
#include <algorithm>
#include <memory>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/language.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

void blaise_parser::parse(blaise_parser_context &context) const
{
   unsigned int index = 0;
   if (is_token(context, blaise_token_type::PROGRAM))
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

   match_token(context, blaise_token_type::PROGRAM);

   auto module = std::make_shared<language::blaise_module>(context.peek_token(), language::blaise_module_type::PROGRAM);
   module->add_subroutine(context.peek_token());
   auto main_subroutine = module->get_subroutine(context.peek_token());

   context.module(module);
   context.main_subroutine(main_subroutine);

   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::SEMICOLON);

   // TODO: match uses
   // TODO: match functions and procedures
   // TODO: match constants

   context.current_subroutine(main_subroutine);
   parse_variables_declaration(context);
   parse_compound_statement(context);

   match_token(context, blaise_token_type::PERIOD);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_program" << std::endl);
}

void blaise_parser::parse_variables_declaration(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variables_declaration" << std::endl);

   if (!is_token(context, blaise_token_type::VAR))
      return; // No such variables
   match_token(context, blaise_token_type::VAR);
   while (is_token(context, blaise_token_type::IDENTIFIER) &&
          (is_token(context, blaise_token_type::COLON, 1) || is_token(context, blaise_token_type::COMMA, 1)))
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
   match_token(context, blaise_token_type::COLON);
   parse_variable_type(context);
   match_token(context, blaise_token_type::SEMICOLON);
   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_declaration" << std::endl);
}

void blaise_parser::parse_variable_names_list(blaise_parser_context &context, std::vector<std::string> &variable_names)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_names_list" << std::endl);

   // TODO: Possible generate a new variable definition, not just the name
   do
   {
      variable_names.push_back(match_token(context, blaise_token_type::IDENTIFIER));
   } while (is_token_and_match(context, blaise_token_type::COMMA));

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_names_list" << std::endl);
}

void blaise_parser::parse_variable_type(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_type" << std::endl);

   const auto token = context.peek_token();
   const auto token_type = token.type();
   // if (is_unsigned && !blaise_token_type_utility::is_unsigned_type(token_type))
   //    throw parser_error("unexpected type after UNSIGNED keyword");
   // else 
   if (!blaise_token_type_utility::is_type(token_type))
      throw parser_error(token.line(), token.column(), "unexpected type");
   match_token(context, token_type);

   // TODO: Return the type definition
   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_type" << std::endl);
}

