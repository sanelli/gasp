#include <vector>
#include <algorithm>
#include <memory>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/ast.hpp>

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

   auto module = std::make_shared<ast::blaise_module>(context.peek_token(), ast::blaise_module_type::PROGRAM);
   module->self(module);
   auto main_subroutine = module->add_subroutine(context.peek_token());

   context.module(module);
   context.main_subroutine(main_subroutine);

   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::SEMICOLON);

   // TODO: match uses
   
   parse_subroutines_declaration(context);

   context.current_subroutine(main_subroutine);
   // TODO: match constants
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

   std::vector<token<blaise_token_type>> variable_tokens;
   parse_variable_names_list(context, variable_tokens);
   match_token(context, blaise_token_type::COLON);
   const auto type_token = parse_variable_type(context);
   match_token(context, blaise_token_type::SEMICOLON);

   auto current_subroutine = context.current_subroutine();

   GASP_DEBUG("blaise-parser", "[INSIDE] Creating " <<  variable_tokens.size() << " variable(s)." << std::endl)
   for(auto variable_token : variable_tokens) {
      GASP_DEBUG("blaise-parser", "[INSIDE] Creating variable '" << variable_token.value() << "' and type '" << type_token.type() << "'" << std::endl)
      current_subroutine->add_variable(variable_token, type_token);
      GASP_DEBUG("blaise-parser", "[INSIDE] Created variable '" << variable_token.value() << "' and type '" << type_token.type() << "'" << std::endl)
   }

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_declaration" << std::endl);
}

void blaise_parser::parse_variable_names_list(blaise_parser_context &context, std::vector<token<blaise_token_type>> &variable_names)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_names_list" << std::endl);

   // TODO: Possible generate a new variable definition, not just the name
   do
   {
      auto token = context.peek_token();
      match_token(context, blaise_token_type::IDENTIFIER);
      variable_names.push_back(token);
   } while (is_token_and_match(context, blaise_token_type::COMMA));

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_names_list" << std::endl);
}

token<blaise_token_type> blaise_parser::parse_variable_type(blaise_parser_context &context)
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

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_type" << std::endl);

   return token;
}

