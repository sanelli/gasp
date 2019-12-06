#include <memory>
#include <string>
#include <vector>

#include <gasp/blaise/ast.hpp>

#include <gasp/blaise/parser/parser.hpp>
#include <sanelli/sanelli.hpp>

using namespace std;
using namespace gasp::blaise;

void blaise_parser::parse_subroutines_declaration(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_declaration" << std::endl);

   do
   {
      auto token = context.peek_token();
      auto token_type = token.type();

      if (token_type == blaise_token_type::FUNCTION)
      {
         parse_function_declaration(context);
      }
      else if (token_type == blaise_token_type::PROCEDURE)
      {
         parse_procedure_declaration(context);
      }
      else if (token_type == blaise_token_type::DECLARE)
      {
         match_token(context, blaise_token_type::DECLARE);
         token = context.peek_token();
         sanelli::token<blaise_token_type> subroutine_token_identifier;
         std::vector<std::shared_ptr<ast::blaise_ast_type>> param_types;
         switch (token.type())
         {
         case blaise_token_type::FUNCTION:
            parse_subroutine_signature(context, blaise_token_type::FUNCTION, true, subroutine_token_identifier, param_types);
            break;
         case blaise_token_type::PROCEDURE:
            parse_subroutine_signature(context, blaise_token_type::PROCEDURE, true, subroutine_token_identifier, param_types);
            break;
         default:
            throw ast::blaise_ast_error(token.line(), token.column(),
                                        sanelli::make_string("Expected 'procedure' or 'function' after 'declare' but found '", token.type(), "'"));
         }
      }
      else // No more subroutines
         break;
   } while (true);

   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_declaration" << std::endl);
}

void inline blaise_parser::parse_subroutine_signature(blaise_parser_context &context,
                                                      blaise_token_type expected_token_type,
                                                      bool is_pure_declaration,
                                                      sanelli::token<blaise_token_type> &subroutine_token_identifier,
                                                      std::vector<std::shared_ptr<ast::blaise_ast_type>> &param_types)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_signature" << std::endl);

   match_token(context, expected_token_type);
   bool is_native = is_token_and_match(context, blaise_token_type::NATIVE);

   subroutine_token_identifier = context.peek_token();
   auto subroutine = context.module()->add_subroutine(subroutine_token_identifier);
   context.current_subroutine(subroutine);

   if (is_native)
      context.current_subroutine()->set(ast::blaise_ast_subroutine_flags::NATIVE);

   match_token(context, blaise_token_type::IDENTIFIER);

   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   parse_subroutine_parameters(context, param_types);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   if (expected_token_type == blaise_token_type::FUNCTION)
   {
      match_token(context, blaise_token_type::COLON);
      auto return_type = parse_variable_type(context, true);
      context.current_subroutine()->return_type(return_type);

      // By definition a function has a variable name with the name
      // of the function and its return type
      context.current_subroutine()->add_variable(subroutine_token_identifier, subroutine_token_identifier.value(), return_type);
   }

   match_token(context, blaise_token_type::SEMICOLON);

   // Check if any other subroutine with the same signature exists
   auto already_defined_subroutine = context.module()->get_exact_subroutine(subroutine_token_identifier, param_types);
   if (already_defined_subroutine != nullptr)
   {

      // If only itslef is present (i.e. it has not been declared before then do not do anything)
      if(subroutine.get() != already_defined_subroutine.get()) {

         if(is_pure_declaration)
            throw ast::blaise_ast_error(subroutine_token_identifier.line(), subroutine_token_identifier.column(),
                                       sanelli::make_string("Subroutine with signature '", subroutine->signature_as_string(), "' has already been declared."));
      
      if(already_defined_subroutine->defined())
         throw ast::blaise_ast_error(subroutine_token_identifier.line(), subroutine_token_identifier.column(),
                                       sanelli::make_string("Subroutine with signature '", subroutine->signature_as_string(), "' does not need to be declared because it has already been defined."));

      // Check return types matches
         if (!already_defined_subroutine->return_type()->equals(subroutine->return_type()))
            throw ast::blaise_ast_error(subroutine_token_identifier.line(), subroutine_token_identifier.column(),
                                       sanelli::make_string("Duplicated subroutine with signature '", subroutine->signature_as_string(), "' but return type differ."));

         // Check parameter names matches
         for(auto param_index = 0; param_index < subroutine->count_parameters(); ++param_index) { 
            auto param_already = already_defined_subroutine->get_parameter(param_index);
            auto param_subroutine = subroutine->get_parameter(param_index);
            if(param_already->name() != param_subroutine->name())
               throw ast::blaise_ast_error(subroutine_token_identifier.line(), subroutine_token_identifier.column(),
                                       sanelli::make_string("Subroutine '", subroutine->signature_as_string(), "' has been declared vefore with different parameter name ('",param_already->name(),"' and '",param_subroutine->name(),"')."));
         }

         // Remove the one that has just been added
         // Set the current subroutine at the old one
         context.module()->remove_last_subroutine();
         context.current_subroutine(already_defined_subroutine);
      }
   }

   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_signature" << std::endl);
}

void inline blaise_parser::parse_subroutine_declaration_impl(blaise_parser_context &context, blaise_token_type expected_token_type, const char *caller_name)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::" << caller_name << std::endl);

   sanelli::token<blaise_token_type> subroutine_token_identifier;
   std::vector<std::shared_ptr<ast::blaise_ast_type>> param_types;
   parse_subroutine_signature(context, expected_token_type, false, subroutine_token_identifier, param_types);

   context.current_subroutine()->define();

   parse_constants_declaration(context);
   parse_variables_declaration(context);

   if (!context.current_subroutine()->is(ast::blaise_ast_subroutine_flags::NATIVE))
   {
      auto compound_statement = parse_compound_statement(context);
      context.current_subroutine()->push_back(compound_statement);
      match_token(context, blaise_token_type::SEMICOLON);
   }

   if (context.module()->count_subroutine(subroutine_token_identifier, param_types) > 1)
   {
      auto subroutine = context.module()->expect_exact_subroutine(subroutine_token_identifier, param_types);
      throw ast::blaise_ast_error(subroutine_token_identifier.line(), subroutine_token_identifier.column(),
                                  sanelli::make_string("Duplicated subroutine with signature '", subroutine->signature_as_string(), "'"));
   }

   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::" << caller_name << std::endl);
}

void blaise_parser::parse_function_declaration(blaise_parser_context &context)
{
   parse_subroutine_declaration_impl(context, blaise_token_type::FUNCTION, "parse_function_declaration");
}

void blaise_parser::parse_procedure_declaration(blaise_parser_context &context)
{
   parse_subroutine_declaration_impl(context, blaise_token_type::PROCEDURE, "parse_procedure_declaration");
}

void blaise_parser::parse_subroutine_parameters(blaise_parser_context &context, std::vector<std::shared_ptr<ast::blaise_ast_type>> &param_types)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_parameters" << std::endl);

   do
   {
      auto token = context.peek_token();
      auto token_type = token.type();

      if (token_type == blaise_token_type::IDENTIFIER)
      {
         vector<sanelli::token<blaise_token_type>> names;
         parse_variable_names_list(context, names);
         match_token(context, blaise_token_type::COLON);
         auto parameters_type = parse_variable_type(context, true);

         for (const auto &name : names)
         {
            auto parameter = context.current_subroutine()->add_parameter(name, name.value(), parameters_type);
            param_types.push_back(parameter->type());
         }
      }
      else
         break;
   } while (is_token_and_match(context, blaise_token_type::COMMA));

   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_parameters" << std::endl);
}
