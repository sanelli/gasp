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

void blaise_parser::parse_function_declaration(blaise_parser_context &context) {
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_function_declaration" << std::endl);

   match_token(context, blaise_token_type::FUNCTION);
   bool is_native = is_token_and_match(context, blaise_token_type::NATIVE);
   auto function_name = match_token_and_get_value(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   parse_subroutine_parameters(context);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   match_token(context, blaise_token_type::COLON);
   auto return_type = parse_variable_type(context);

   if(!is_native)
      parse_compound_statement(context);
   
   match_token(context, blaise_token_type::SEMICOLON);

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_function_declaration" << std::endl);
}

void blaise_parser::parse_procedure_declaration(blaise_parser_context &context) {
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_procedure_declaration" << std::endl);

   match_token(context, blaise_token_type::PROCEDURE);
   bool is_native = is_token_and_match(context, blaise_token_type::NATIVE);
   auto function_name = match_token_and_get_value(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   parse_subroutine_parameters(context);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   if(!is_native)
      parse_compound_statement(context);
   
   match_token(context, blaise_token_type::SEMICOLON);
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_procedure_declaration" << std::endl);
}

void blaise_parser::parse_subroutine_parameters(blaise_parser_context &context) {
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_parameters" << std::endl);

   do {

      auto token = context.peek_token();
      auto token_type = token.type();

      if(token_type == blaise_token_type::IDENTIFIER){
         vector<gasp::common::token<blaise_token_type>> parameters;
         parse_variable_names_list(context, parameters);
         match_token(context, blaise_token_type::COLON);
         auto parameters_type = parse_variable_type(context);
      } else
            break;
   } while(is_token_and_match(context, blaise_token_type::COMMA));

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_parameters" << std::endl);
}

