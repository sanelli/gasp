#include <memory>
#include <string>

#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/parser.hpp>
#include <gasp/blaise/ast.hpp>

using namespace std;
using namespace gasp::blaise;
using namespace gasp::common;

// Algorihtm for expression management is taked from
// https://en.wikipedia.org/wiki/Operator-precedence_parser
shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_expression(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression" << std::endl);

   const auto lhs = parse_expression_term(context);
   const auto expression = parse_expression_helper(context, lhs, 0);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression" << std::endl);

   return expression;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_expression_helper(blaise_parser_context &context, shared_ptr<ast::blaise_ast_expression> lhs, unsigned int min_precedence)
{

   GASP_DEBUG("blaise-parser", make_string("[ENTER] blaise_parser::parse_expression<", min_precedence, ">") << std::endl);

   token<blaise_token_type> lookahead_token = context.peek_token();
   while (blaise_token_type_utility::is_operator(lookahead_token.type()) && blaise_token_type_utility::get_operator_precedence(lookahead_token.type()) >= min_precedence)
   {
      auto operator_token = lookahead_token;
      match_token(context, operator_token.type());
      auto rhs = parse_expression_term(context);
      lookahead_token = std::move(context.peek_token());
      while (blaise_token_type_utility::is_operator(lookahead_token.type()) && blaise_token_type_utility::get_operator_precedence(lookahead_token.type()) > blaise_token_type_utility::get_operator_precedence(operator_token.type()))
      {
         rhs = parse_expression_helper(context, rhs, blaise_token_type_utility::get_operator_precedence(lookahead_token.type()));
         lookahead_token = std::move(context.peek_token());
      }
      lhs = ast::blaise_ast_expression_binary_factory(lhs, operator_token, rhs);
   }
   GASP_DEBUG("blaise-parser", make_string("[EXIT] blaise_parser::parse_expression<", min_precedence, ">") << std::endl);
   return lhs;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_expression_term(blaise_parser_context &context)
{
   shared_ptr<ast::blaise_ast_expression> term_expression = nullptr;

   const auto token = context.peek_token();
   const auto token_type = token.type();

   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression_term<" << token_type << ">" << std::endl);

   switch (token_type)
   {
   case blaise_token_type::IDENTIFIER: // Identifier could be either a variable or a function call
   {
      auto identifier_token = context.peek_token();
      match_token(context, blaise_token_type::IDENTIFIER);
      const auto lookahead = context.peek_token().type();
      if (is_token_and_match(context, blaise_token_type::LEFT_PARENTHESES))
      {
         vector<shared_ptr<ast::blaise_ast_expression>> expressions;
         vector<ast::blaise_ast_type> types;
         parse_subroutine_call_parameters(context, expressions, types);
         match_token(context, blaise_token_type::RIGHT_PARENTHESES);

         // Look up for the expression function
         // TODO: Add ability to look into referenced modules when the name contains a dot
         auto subroutine = context.module()->get_subroutine(identifier_token, types);
         if (subroutine == nullptr)
            throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                           make_string("Cannot find a function matching: '", identifier_token.value(), "(", types, ")'"));
         term_expression = ast::blaise_ast_expression_subroutine_call_factory(identifier_token, subroutine, expressions);
      }
      else
      {
         term_expression = ast::blaise_ast_expression_memory_location_factory(context.current_subroutine(), token);
      }
   }
   break;
   case blaise_token_type::MINUS: // it is a negated sub expression
   {
      match_token(context, blaise_token_type::MINUS);
      auto expression = parse_expression(context);
      term_expression = ast::blaise_ast_expression_unary_factory(token, expression);
   }
   break;
   case blaise_token_type::LOGICAL_NOT: // it is a negated sub expression
   {
      match_token(context, blaise_token_type::LOGICAL_NOT);
      auto expression = parse_expression(context);
      term_expression = ast::blaise_ast_expression_unary_factory(token, expression);
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
      term_expression = blaise_ast_expression_value_factory(context, token);
      match_token(context, token_type);
   }
   break;
   case blaise_token_type::CAST:
   {
      term_expression = parse_cast_expression(context);
   }
   break;
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("Unexpected token '", token_type, "' found."));
   }

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression_term<" << token_type << ">" << std::endl);
   return term_expression;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_number(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_number" << std::endl);

   shared_ptr<ast::blaise_ast_expression> number_literal = nullptr;

   auto token = context.peek_token();
   auto token_type = token.type();
   if (!blaise_token_type_utility::is_number(token_type))
      throw_parse_error_with_details(context, token.line(), token.column(), make_string("A number was expected but found '", token_type, "'"));

   number_literal = blaise_ast_expression_value_factory(context, token);
   match_token(context, token_type);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_number" << std::endl);
   return number_literal;
}

shared_ptr<ast::blaise_ast_expression> gasp::blaise::blaise_ast_expression_value_factory(blaise_parser_context &context, const token<blaise_token_type> &token_literal)
{
   switch (token_literal.type())
   {
   case blaise_token_type::INTEGER_LITERAL:
      return make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value()));
   case blaise_token_type::INTEGER_BASE_TWO_LITERAL:
      return make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value().substr(2), nullptr, 2));
   case blaise_token_type::INTEGER_BASE_EIGHT_LITERAL:
      return make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value().substr(2), nullptr, 8));
   case blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL:
      return make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value().substr(2), nullptr, 16));
   case blaise_token_type::FLOAT_LITERAL:
      return make_shared<ast::blaise_ast_expression_float_value>(token_literal, stof(token_literal.value()));
   case blaise_token_type::DOUBLE_LITERAL:
      return make_shared<ast::blaise_ast_expression_double_value>(token_literal, stod(token_literal.value()));
   case blaise_token_type::CHAR_LITERAL:
      return make_shared<ast::blaise_ast_expression_char_value>(token_literal, token_literal.value()[1]);
   case blaise_token_type::STRING_LITERAL:
      return make_shared<ast::blaise_ast_expression_string_value>(token_literal, token_literal.value().substr(1, token_literal.value().length() - 2));
   case blaise_token_type::BOOLEAN_LITERAL:
      return make_shared<ast::blaise_ast_expression_boolean_value>(token_literal, token_literal.value() == "true");
   default:
      gasp::blaise::blaise_parser::throw_parse_error_with_details(context, token_literal.line(), token_literal.column(), make_string("Cannot extract literal from token '", token_literal, "'"));
   }
}

// NOTE: I have a specific parser for boolean becaus ein the future I would like to add
//       the triboolean type (true, false, undefined)
shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_boolean(blaise_parser_context &context)
{
   GASP_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_boolean" << std::endl);

   shared_ptr<ast::blaise_ast_expression> boolean_literal = nullptr;

   auto literal_token = context.peek_token();
   auto token_type = literal_token.type();
   if (!blaise_token_type_utility::is_boolean(token_type))
      throw_parse_error_with_details(context, literal_token.line(), literal_token.column(), make_string("A boolean was expected but found '", token_type, "'"));

   boolean_literal = blaise_ast_expression_value_factory(context, literal_token);
   match_token(context, token_type);

   GASP_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_boolean" << std::endl);
   return boolean_literal;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_cast_expression(blaise_parser_context &context)
{
   auto reference = context.peek_token();
   match_token(context, blaise_token_type::CAST);
   match_token(context, blaise_token_type::LESS_THAN);
   auto return_type_token = parse_variable_type(context);
   auto return_type = ast::get_type_from_token(return_type_token);
   match_token(context, blaise_token_type::GREAT_THAN);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   auto expression = parse_expression(context);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);
   if (!ast::blaise_ast_utility::can_force_cast(expression->result_type(), return_type))
      throw_parse_error_with_details(context, reference.line(), reference.column(), make_string("It is not possible force a cast from '", expression->result_type(), "' to '", return_type, "'."));
   return make_shared<ast::blaise_ast_expression_cast>(reference, return_type, expression);
}
