#include <memory>
#include <string>
#include <vector>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/parser.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/language.hpp>
#include <gasp/blaise/parser.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

void blaise_parser::parse_subroutines_declaration(blaise_parser_context &context) {
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_declaration" << std::endl);

   do {
   auto token = context.peek_token();
   auto token_type = token.type();

   if(token_type == blaise_token_type::FUNCTION){
      parse_function_declaration(context);
   } else if(token_type == blaise_token_type::PROCEDURE){
      parse_procedure_declaration(context);
   } else // No more subroutines
         break;
   }while(true);

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_declaration" << std::endl);
}

void inline blaise_parser::parse_subroutine_declaration_impl(blaise_parser_context &context, blaise_token_type expected_token_type, const char* caller_name) {
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::" << caller_name << std::endl);

   match_token(context, expected_token_type);
   bool is_native = is_token_and_match(context, blaise_token_type::NATIVE);

   auto subroutine_token_identifier = context.peek_token();
   auto subroutine = context.module()->add_subroutine(subroutine_token_identifier);
   context.current_subroutine(subroutine);

   if(is_native)
      context.current_subroutine()->set(language::blaise_subroutine_flags::NATIVE);

   match_token(context, blaise_token_type::IDENTIFIER);

   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   std::vector<language::blaise_language_type> param_types;
   parse_subroutine_parameters(context, param_types);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   if(expected_token_type == blaise_token_type::FUNCTION) {
      match_token(context, blaise_token_type::COLON);
      auto return_type = parse_variable_type(context);
      context.current_subroutine()->return_type(return_type);
   }
   parse_variables_declaration(context);

   if(!is_native)
      parse_compound_statement(context);
   
   match_token(context, blaise_token_type::SEMICOLON);

   if(context.module()->count_subroutine(subroutine_token_identifier, param_types) > 1) { 
      auto subroutine = context.module()->get_subroutine(subroutine_token_identifier, param_types);
      throw language::blaise_language_error(subroutine_token_identifier.line(), subroutine_token_identifier.column(), 
               make_string("Duplicated subroutine with signature '", subroutine->signature_as_string() ,"'"));
   }

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::" << caller_name << std::endl);
}

void blaise_parser::parse_function_declaration(blaise_parser_context &context) {
   parse_subroutine_declaration_impl(context, blaise_token_type::FUNCTION, "parse_function_declaration");
}

void blaise_parser::parse_procedure_declaration(blaise_parser_context &context) {
   parse_subroutine_declaration_impl(context, blaise_token_type::PROCEDURE, "parse_procedure_declaration");
}

void blaise_parser::parse_subroutine_parameters(blaise_parser_context &context, std::vector<language::blaise_language_type>& param_types) {
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_parameters" << std::endl);

   do {
      auto token = context.peek_token();
      auto token_type = token.type();

      if(token_type == blaise_token_type::IDENTIFIER){
         vector<gasp::common::token<blaise_token_type>> names;
         parse_variable_names_list(context, names);
         match_token(context, blaise_token_type::COLON);
         auto parameters_type = parse_variable_type(context);

         for(const auto& name : names)
         {
            auto parameter = context.current_subroutine()->add_parameter(name, parameters_type);
            param_types.push_back(parameter->type());
         }

      } else
            break;
   } while(is_token_and_match(context, blaise_token_type::COMMA));

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_parameters" << std::endl);
}

