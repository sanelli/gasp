#include <vector>
#include <algorithm>
#include <memory>

#include <gasp/common/tokenizer.hpp>
#include <sanelli/sanelli.hpp>

#include <gasp/blaise/tokenizer/tokens.hpp>
#include <gasp/blaise/parser/parser.hpp>
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
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_program" << std::endl);

   match_token(context, blaise_token_type::PROGRAM);

   auto identifier = context.peek_token();
   auto module = ast::make_blaise_ast_module(identifier, identifier.value(), ast::blaise_ast_module_type::PROGRAM);
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

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_program" << std::endl);
}

void blaise_parser::parse_variables_declaration(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variables_declaration" << std::endl);

   if (!is_token(context, blaise_token_type::VAR))
      return; // No such variables
   match_token(context, blaise_token_type::VAR);
   while (is_token(context, blaise_token_type::IDENTIFIER) &&
          (is_token(context, blaise_token_type::COLON, 1) || is_token(context, blaise_token_type::COMMA, 1)))
   {
      parse_variable_declaration(context);
   }

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variables_declaration" << std::endl);
}

void blaise_parser::parse_variable_declaration(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_declaration" << std::endl);

   std::vector<token<blaise_token_type>> variable_tokens;
   parse_variable_names_list(context, variable_tokens);
   match_token(context, blaise_token_type::COLON);
   const auto variable_type = parse_variable_type(context, true);
   
   match_token(context, blaise_token_type::SEMICOLON);

   auto current_subroutine = context.current_subroutine();

   SANELLI_DEBUG("blaise-parser", "[INSIDE] Creating " <<  variable_tokens.size() << " variable(s)." << std::endl)
   for(auto variable_token : variable_tokens) {
      SANELLI_DEBUG("blaise-parser", "[INSIDE] Creating variable '" << variable_token.value() << "' and type '" << variable_type << "'" << std::endl);
      current_subroutine->add_variable(variable_token, variable_token.value(), variable_type);
      SANELLI_DEBUG("blaise-parser", "[INSIDE] Done created variable '" << variable_token.value() << "' and type '" << variable_type << "'" << std::endl);
   }

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_declaration" << std::endl);
}

void blaise_parser::parse_variable_names_list(blaise_parser_context &context, std::vector<token<blaise_token_type>> &variable_names)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_names_list" << std::endl);

   // TODO: Possible generate a new variable definition, not just the name
   do
   {
      auto token = context.peek_token();
      match_token(context, blaise_token_type::IDENTIFIER);
      variable_names.push_back(token);
   } while (is_token_and_match(context, blaise_token_type::COMMA));

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_names_list" << std::endl);
}

std::shared_ptr<ast::blaise_ast_type> blaise_parser::parse_variable_type(blaise_parser_context &context, bool accept_unbounded_array)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_variable_type" << std::endl);

   const auto type_token = context.peek_token();
   const auto token_type = type_token.type();
   // if (is_unsigned && !blaise_token_type_utility::is_unsigned_type(token_type))
   //    throw parser_error("unexpected type after UNSIGNED keyword");
   // else 
   if (!blaise_token_type_utility::is_type(token_type))
      throw parser_error(type_token.line(), type_token.column(), "unexpected type");
   match_token(context, token_type);

   std::shared_ptr<ast::blaise_ast_type> variable_type = nullptr;
   if(type_token.type() == blaise_token_type::TYPE_ARRAY) { 
      match_token(context, blaise_token_type::LESS_THAN);
      auto inner_type = parse_variable_type(context, true);
      match_token(context, blaise_token_type::GREAT_THAN);
      auto array_size_length = 0;
      if(is_token_and_match(context, blaise_token_type::LEFT_BRACKET)){
         auto array_size_expression = parse_number(context);
         if(!ast::blaise_ast_utility::is_integer(array_size_expression->result_type()))
            throw_parse_error_with_details(context, "An integral value was expected as array size");
         match_token(context, blaise_token_type::RIGHT_BRACKET);
         auto array_size = std::static_pointer_cast<ast::blaise_ast_expression_integer_value>(array_size_expression);
         array_size_length = array_size->value();
      } else {
         SANELLI_DEBUG("blaise-parser", "[INSIDE] blaise_parser::parse_variable_type accept_unbounded_array = " << (accept_unbounded_array ? "TRUE" : "FALSE") << std::endl);
         if(!accept_unbounded_array)
            throw_parse_error_with_details(context, "Array size required");
      }

      variable_type = ast::get_array_type_from_token(type_token, inner_type, array_size_length, accept_unbounded_array);
   }else {
      // Just a plain variable
      variable_type = ast::get_type_from_token(type_token);
   }

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_type" << std::endl);

   return variable_type;
}

