#include <memory>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/language.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

// Algorihtm for expression management is taked from
// https://en.wikipedia.org/wiki/Operator-precedence_parser
shared_ptr<language::blaise_expression> blaise_parser::parse_expression(blaise_parser_context &context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression" << std::endl);

   const auto lhs = parse_expression_term(context);
   const auto expression = parse_expression_helper(context, lhs, 0);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression" << std::endl);

   return expression;
}

shared_ptr<language::blaise_expression> blaise_parser::parse_expression_helper(blaise_parser_context &context, shared_ptr<language::blaise_expression> lhs, unsigned int min_precedence){

   GASP_DEBUG("blaise-parser", make_string("[ENTER] blaise_parser::parse_expression<",min_precedence,">") << std::endl);

   token<blaise_token_type> lookahead_token = context.peek_token();
   while(blaise_token_type_utility::is_operator(lookahead_token.type()) 
            && blaise_token_type_utility::get_operator_precedence(lookahead_token.type()) >= min_precedence){
      auto operator_token = lookahead_token;
      match_token(context, operator_token.type());
      auto rhs = parse_expression_term(context);
      lookahead_token = std::move(context.peek_token());
      while(blaise_token_type_utility::is_operator(lookahead_token.type())
            && blaise_token_type_utility::get_operator_precedence(lookahead_token.type()) > blaise_token_type_utility::get_operator_precedence(operator_token.type()) ){
               rhs =  parse_expression_helper (context, rhs, blaise_token_type_utility::get_operator_precedence(lookahead_token.type()));
               lookahead_token = std::move(context.peek_token());
            }
      lhs = language::blaise_expression_binary_factory(lhs, operator_token, rhs);
   } 
   GASP_DEBUG("blaise-parser", make_string("[EXIT] blaise_parser::parse_expression<",min_precedence,">") << std::endl);
   return lhs;
}

shared_ptr<language::blaise_expression> blaise_parser::parse_expression_term(blaise_parser_context &context)
{
   shared_ptr<language::blaise_expression> term_expression = nullptr;

   const auto token = context.peek_token();
   const auto token_type = token.type();

  GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression_term<" << token_type << ">" << std::endl);

   switch (token_type)
   {
   case blaise_token_type::IDENTIFIER: // Identifier could be either a variable or a function call
      {
         match_token(context, blaise_token_type::IDENTIFIER);
         const auto lookahead = context.peek_token().type();
         if(is_token_and_match(context, blaise_token_type::LEFT_PARENTHESES)){
            parse_function_call_parameters(context);
            match_token(context, blaise_token_type::RIGHT_PARENTHESES);
            // TODO: Create an expression for the function call
         } else {
            term_expression = language::blaise_expression_memory_location_factory(context.current_subroutine(), token);
         }
      }
      break;
   case blaise_token_type::MINUS: // it is a negated sub expression
      {
         match_token(context, blaise_token_type::MINUS);
         auto expression = parse_expression(context);
         term_expression = language::blaise_expression_unary_factory(token, expression);
      }
      break;
   case blaise_token_type::LOGICAL_NOT: // it is a negated sub expression
      {
         match_token(context, blaise_token_type::LOGICAL_NOT);
         auto expression = parse_expression(context);
         term_expression = language::blaise_expression_unary_factory(token, expression);
      }
      break;
   case blaise_token_type::INTEGER_LITERAL: // It is a number
   case blaise_token_type::DOUBLE_LITERAL: 
   case blaise_token_type::FLOAT_LITERAL: 
   case blaise_token_type::INTEGER_BASE_TWO_LITERAL: 
   case blaise_token_type::INTEGER_BASE_EIGHT_LITERAL:
   case blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL:
      {
         term_expression = parse_number(context);
      }
      break;
   case blaise_token_type::LEFT_PARENTHESES: // sub expression between parenthesis
      {
         match_token(context, blaise_token_type::MINUS);
         term_expression = parse_expression(context);
         match_token(context, blaise_token_type::RIGHT_PARENTHESES);
      }
      break;
   case blaise_token_type::BOOLEAN_LITERAL:
      {
         term_expression = parse_boolean(context);
      }
      break;
   case blaise_token_type::STRING_LITERAL:
   case blaise_token_type::CHAR_LITERAL:
      {
         term_expression = language::blaise_expression_value_factory(token);
         match_token(context, token_type);
      }
      break;
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found."));
   }

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression_term<" << token_type << ">" << std::endl);
   return term_expression;
}

shared_ptr<language::blaise_expression> blaise_parser::parse_number(blaise_parser_context& context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_number" << std::endl);

   shared_ptr<language::blaise_expression> number_literal = nullptr;

   auto token = context.peek_token();
   auto token_type = token.type();
   if(!blaise_token_type_utility::is_number(token_type))
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("A number was expected but found '", token_type, "'"));

   number_literal = language::blaise_expression_value_factory(token);
   match_token(context, token_type);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_number" << std::endl);
   return number_literal;
}

// NOTE: I have a specific parser for boolean becaus ein the future I would like to add
//       the triboolean type (true, false, undefined)
shared_ptr<language::blaise_expression> blaise_parser::parse_boolean(blaise_parser_context& context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_boolean" << std::endl);

   shared_ptr<language::blaise_expression> boolean_literal = nullptr;

   auto token = context.peek_token();
   auto token_type = token.type();
   if(!blaise_token_type_utility::is_boolean(token_type))
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("A boolean was expected but found '", token_type, "'"));

   boolean_literal = language::blaise_expression_value_factory(token);
   match_token(context, token_type);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_boolean" << std::endl);
   return boolean_literal;
}