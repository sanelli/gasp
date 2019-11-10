#include <vector>
#include <algorithm>
#include <memory>
#include <set>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>

using namespace sanelli;
using namespace std;
using namespace gasp::blaise;

void blaise_parser::parse(blaise_parser_context &context) const
{
   unsigned int index = 0;
   if (is_token(context, blaise_token_type::PROGRAM))
      parse_program(context);
   else if (is_token(context, blaise_token_type::MODULE))
      parse_module(context);

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
   main_subroutine->set(blaise::ast::blaise_ast_subroutine_flags::MAIN);

   context.module(module);
   context.main_subroutine(main_subroutine);

   match_token(context, blaise_token_type::IDENTIFIER);

   // Main program input variables
   if (is_token_and_match(context, blaise_token_type::LEFT_PARENTHESES))
   {
      context.current_subroutine(main_subroutine);
      std::vector<std::shared_ptr<ast::blaise_ast_type>> param_types;
      parse_subroutine_parameters(context, param_types);
      match_token(context, blaise_token_type::RIGHT_PARENTHESES);
      context.current_subroutine(nullptr);
   }

   // Retrun value of the main program
   auto return_type = ast::make_plain_type(ast::blaise_ast_system_type::INTEGER);
   if (is_token_and_match(context, blaise_token_type::COLON))
   {
      return_type = parse_variable_type(context, false);
   }

   main_subroutine->return_type(return_type);
   main_subroutine->add_variable(identifier, identifier.value(), return_type);

   match_token(context, blaise_token_type::SEMICOLON);

   parse_uses_declaration(context);

   parse_subroutines_declaration(context);

   context.current_subroutine(main_subroutine);

   parse_constants_declaration(context);
   parse_variables_declaration(context);

   auto compound_statement = parse_compound_statement(context);
   main_subroutine->push_back(compound_statement);

   match_token(context, blaise_token_type::PERIOD);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_program" << std::endl);
}

void blaise_parser::parse_module(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_module" << std::endl);

   match_token(context, blaise_token_type::MODULE);

   auto identifier = context.peek_token();
   auto module = ast::make_blaise_ast_module(identifier, identifier.value(), ast::blaise_ast_module_type::MODULE);
   module->self(module);
   context.module(module);

   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::SEMICOLON);
   parse_uses_declaration(context);

   parse_subroutines_declaration(context);

   match_token(context, blaise_token_type::END);
   match_token(context, blaise_token_type::PERIOD);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_module" << std::endl);
}

void blaise_parser::parse_uses_declaration(blaise_parser_context &context)
{
   while (is_token(context, blaise_token_type::USE))
   {
      match_token(context, blaise_token_type::USE);
      do
      {
         auto identifier = context.peek_token();
         auto dependency = match_token_and_get_value(context, blaise_token_type::IDENTIFIER);
         auto dependency_module = context.get_dependency(dependency);
         if (dependency_module == nullptr)
            throw_parse_error_with_details(context, sanelli::make_string("Cannot load blaise module '", dependency, "'."));
         context.module()->add_dependency(dependency_module);
      } while (is_token_and_match(context, blaise_token_type::COMMA));
      match_token(context, blaise_token_type::SEMICOLON);
   }
}

void blaise_parser::parse_constants_declaration(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_constants_declaration" << std::endl);

   if (!is_token(context, blaise_token_type::CONST))
      return; // No such variables
   match_token(context, blaise_token_type::CONST);
   while (is_token(context, blaise_token_type::IDENTIFIER))
      parse_constant_declaration(context);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_constants_declaration" << std::endl);
}

void blaise_parser::parse_constant_declaration(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_constant_declaration" << std::endl);

   auto identifier_token = context.peek_token();
   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::ASSIGNMENT);

   auto expression = parse_expression(context);
   if(!ast::blaise_ast_utility::is_allowed_for_constant(expression))
      throw_parse_error_with_details(context, expression->line(), expression->column(), 
         "Cannot create a constant. Expression is not a literal.");

   auto current_subroutine = context.current_subroutine();
   auto constant = current_subroutine->add_constant(identifier_token, identifier_token.value(), expression->result_type());
   constant->literal_expression(expression);   

   match_token(context, blaise_token_type::SEMICOLON); 
   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_constant_declaration" << std::endl);
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

   SANELLI_DEBUG("blaise-parser", "[INSIDE] Creating " << variable_tokens.size() << " variable(s)." << std::endl)
   for (auto variable_token : variable_tokens)
   {
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
   if (type_token.type() == blaise_token_type::TYPE_ARRAY)
   {
      match_token(context, blaise_token_type::LESS_THAN);
      auto underlying_type = parse_variable_type(context, true);
      match_token(context, blaise_token_type::GREAT_THAN);
      auto array_size_length = 0;
      if (is_token_and_match(context, blaise_token_type::LEFT_BRACKET))
      {
         auto array_size_expression = parse_number(context);
         if (!ast::blaise_ast_utility::is_integer(array_size_expression->result_type()))
            throw_parse_error_with_details(context, "An integral value was expected as array size");
         match_token(context, blaise_token_type::RIGHT_BRACKET);
         auto array_size = std::static_pointer_cast<ast::blaise_ast_expression_integer_value>(array_size_expression);
         array_size_length = array_size->value();
      }
      else
      {
         SANELLI_DEBUG("blaise-parser", "[INSIDE] blaise_parser::parse_variable_type accept_unbounded_array = " << (accept_unbounded_array ? "TRUE" : "FALSE") << std::endl);
         if (!accept_unbounded_array)
            throw_parse_error_with_details(context, "Array size required");
      }

      variable_type = ast::get_array_type_from_token(type_token, underlying_type, array_size_length, accept_unbounded_array);
   }
   else
   {
      // Just a plain variable
      variable_type = ast::get_type_from_token(type_token);
   }

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_variable_type" << std::endl);

   return variable_type;
}
