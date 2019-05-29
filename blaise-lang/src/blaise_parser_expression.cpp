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
void blaise_parser::parse_expression(blaise_parser_context &context){
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression" << std::endl);

   /* const auto lhs = */ parse_expression_term(context);
   parse_expression_helper(context, /* lhs, */ 0);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression" << std::endl);
}

void blaise_parser::parse_expression_helper(blaise_parser_context &context, /* const auto& lhs, */ unsigned int min_precedence){

   GASP_DEBUG("blaise-parser", make_string("[ENTER] blaise_parser::parse_expression<",min_precedence,">") << std::endl);

   token<blaise_token_type> lookahead_token = context.peek_token();
   while(blaise_token_type_utility::is_operator(lookahead_token.type()) 
            && blaise_token_type_utility::get_operator_precedence(lookahead_token.type()) >= min_precedence){
      auto op = lookahead_token.type();
      match_token(context, op);
      /* auto rhs = */ parse_expression_term(context);
      lookahead_token = std::move(context.peek_token());
      while(blaise_token_type_utility::is_operator(lookahead_token.type())
            && blaise_token_type_utility::get_operator_precedence(lookahead_token.type()) > blaise_token_type_utility::get_operator_precedence(op) ){
               /* rhs = */ parse_expression_helper (context, /* rhs, */ blaise_token_type_utility::get_operator_precedence(lookahead_token.type()));
               lookahead_token = std::move(context.peek_token());
            }
      /* lhs = lhs op rhs */
   } 
   GASP_DEBUG("blaise-parser", make_string("[EXIT] blaise_parser::parse_expression<",min_precedence,">") << std::endl);
   /* return lhs */
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
            term_expression = language::blaise_expression_variable_factory(context.current_subroutine(), token);
         }
      }
      break;
   case blaise_token_type::MINUS: // it is a negated sub expression
      {
         match_token(context, blaise_token_type::MINUS);
         /* result = */ parse_expression(context);
      }
      break;
   case blaise_token_type::LOGICAL_NOT: // it is a negated sub expression
      {
         match_token(context, blaise_token_type::LOGICAL_NOT);
         /* result = */ parse_expression(context);
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
         /* term_expression = */ parse_expression(context);
         match_token(context, blaise_token_type::RIGHT_PARENTHESES);
      }
      break;
   case blaise_token_type::STRING_LITERAL:
      {
         term_expression = language::blaise_expression_value_factory(token);
         match_token(context, blaise_token_type::STRING_LITERAL);
      }
      break;
   case blaise_token_type::CHAR_LITERAL:
      {
         term_expression = language::blaise_expression_value_factory(token);
         match_token(context, blaise_token_type::CHAR_LITERAL);
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