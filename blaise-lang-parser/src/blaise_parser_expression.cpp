#include <memory>
#include <string>
#include <sstream>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>

using namespace sanelli;
using namespace std;
using namespace gasp::blaise;

// Algorihtm for expression management is taked from
// https://en.wikipedia.org/wiki/Operator-precedence_parser
shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_expression(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression" << std::endl);

   const auto lhs = parse_expression_term(context);
   const auto expression = parse_expression_helper(context, lhs, 0);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression" << std::endl);

   return expression;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_expression_helper(blaise_parser_context &context, shared_ptr<ast::blaise_ast_expression> lhs, unsigned int min_precedence)
{
   SANELLI_DEBUG("blaise-parser", sanelli::make_string("[ENTER] blaise_parser::parse_expression<", min_precedence, ">") << std::endl);

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
      if (lhs->result_type() != rhs->result_type())
      {
         if (ast::blaise_ast_utility::can_auto_cast(lhs->result_type(), rhs->result_type()))
            lhs = ast::introduce_cast_if_required(operator_token, rhs->result_type(), lhs);
         else if (ast::blaise_ast_utility::can_auto_cast(rhs->result_type(), lhs->result_type()))
            rhs = ast::introduce_cast_if_required(operator_token, lhs->result_type(), rhs);
         else
            throw_parse_error_with_details(context, lhs->line(), lhs->column(),
                                           sanelli::make_string("Operator ", operator_token, " between expressions of type ", lhs->result_type(),
                                                                " and ", rhs->result_type(), " is not supported."));
      }

      lhs = ast::make_blaise_ast_expression_binary(lhs, operator_token, rhs);
   }
   SANELLI_DEBUG("blaise-parser", sanelli::make_string("[EXIT] blaise_parser::parse_expression<", min_precedence, ">") << std::endl);
   return lhs;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_expression_term(blaise_parser_context &context)
{
   shared_ptr<ast::blaise_ast_expression> term_expression = nullptr;

   const auto token = context.peek_token();
   const auto token_type = token.type();

   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_expression_term<" << token_type << ">" << std::endl);

   switch (token_type)
   {
   case blaise_token_type::IDENTIFIER: // Identifier could be either a variable or a function call
   {
      auto identifier_token = context.peek_token();
      match_token(context, blaise_token_type::IDENTIFIER);
      const auto lookahead = context.peek_token().type();
      if (is_token_and_match(context, blaise_token_type::LEFT_PARENTHESES)) // Function call
      {
         vector<shared_ptr<ast::blaise_ast_expression>> expressions;
         vector<std::shared_ptr<ast::blaise_ast_type>> types;
         parse_subroutine_call_parameters(context, expressions, types);
         match_token(context, blaise_token_type::RIGHT_PARENTHESES);

         // Look up for the expression function
         auto subroutine = context.module()->get_subroutine(identifier_token, types);
         if (subroutine == nullptr)
         {
            std::stringstream types_rep;
            for (auto index = 0; index < types.size(); ++index)
            {
               auto type = types.at(index);
               if (index > 0)
                  types_rep << ", ";
               types_rep << to_string(type);
            }
            throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                           sanelli::make_string("Cannot find a function matching call for '", identifier_token.value(), "(", types_rep.str(), ")'"));
         }

         ast::introduce_cast_if_required(identifier_token, subroutine, expressions);

         term_expression = ast::make_blaise_ast_expression_subroutine_call(identifier_token, subroutine, expressions);
      }
      else if (is_token_and_match(context, blaise_token_type::LEFT_BRACKET))
      { // Array access
         auto indexing_expression = parse_expression(context);
         match_token(context, blaise_token_type::RIGHT_BRACKET);

         auto memory_location = context.current_subroutine()->get_memory_location(identifier_token.value());
         if (memory_location == nullptr)
            throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                           sanelli::make_string("Cannot find array with name '", identifier_token.value(), "'"));
         switch (memory_location->type()->type_type())
         {
         case ast::blaise_ast_type_type::ARRAY: // All good
            break;
         case ast::blaise_ast_type_type::PLAIN:
         case ast::blaise_ast_type_type::POINTER:
         case ast::blaise_ast_type_type::USER_DEFINED:
            throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                           sanelli::make_string("Variable '", identifier_token.value(), "' must be of type array"));
         default:
            throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                           sanelli::make_string("Unexpected variable type '", memory_location->type(), "'."));
         }
         auto integer_type = ast::make_plain_type(ast::blaise_ast_system_type::INTEGER);
         if (indexing_expression->result_type() != integer_type && !ast::blaise_ast_utility::can_auto_cast(indexing_expression->result_type(), integer_type))
         {
            throw_parse_error_with_details(context, indexing_expression->line(), indexing_expression->column(),
                                           sanelli::make_string("indexing expression must be an integer or a type that can be casted to an integer."));
         }
         term_expression = ast::make_blaise_ast_expression_array_access(identifier_token, memory_location, indexing_expression);
      }
      else // Variable access
      {
         auto memory_location = context.current_subroutine()->get_memory_location(identifier_token.value());
         if (memory_location == nullptr)
            throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                           sanelli::make_string("Cannot find variable with name '", identifier_token.value(), "'"));

         term_expression = ast::make_blaise_ast_expression_memory_access(identifier_token, memory_location);
      }
   }
   break;
   case blaise_token_type::MINUS: // it is a negated sub expression
   {
      match_token(context, blaise_token_type::MINUS);
      auto expression = parse_expression(context);
      term_expression = ast::make_blaise_ast_expression_unary(token, expression);
   }
   break;
   case blaise_token_type::LOGICAL_NOT: // it is a negated sub expression
   {
      match_token(context, blaise_token_type::LOGICAL_NOT);
      auto expression = parse_expression(context);
      term_expression = ast::make_blaise_ast_expression_unary(token, expression);
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
      match_token(context, blaise_token_type::LEFT_PARENTHESES);
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
      term_expression = make_blaise_ast_expression_value(context, token);
      match_token(context, token_type);
   }
   break;
   case blaise_token_type::CAST:
   {
      term_expression = parse_cast_expression(context);
   }
   break;
   case blaise_token_type::IF:
   {
      term_expression = parse_ternary_expression(context);
   }
   break;
   case blaise_token_type::NEW:
   {
      term_expression = parse_new_expression(context);
   }
   break;
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), sanelli::make_string("Unexpected token '", token_type, "' found."));
   }

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_expression_term<" << token_type << ">" << std::endl);
   return term_expression;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_number(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_number" << std::endl);

   shared_ptr<ast::blaise_ast_expression> number_literal = nullptr;

   auto token = context.peek_token();
   auto token_type = token.type();
   if (!blaise_token_type_utility::is_number(token_type))
      throw_parse_error_with_details(context, token.line(), token.column(), sanelli::make_string("A number was expected but found '", token_type, "'"));

   number_literal = make_blaise_ast_expression_value(context, token);
   match_token(context, token_type);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_number" << std::endl);
   return number_literal;
}

shared_ptr<ast::blaise_ast_expression> gasp::blaise::make_blaise_ast_expression_value(blaise_parser_context &context, const token<blaise_token_type> &token_literal)
{
   switch (token_literal.type())
   {
   case blaise_token_type::INTEGER_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value()));
   case blaise_token_type::INTEGER_BASE_TWO_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value().substr(2), nullptr, 2));
   case blaise_token_type::INTEGER_BASE_EIGHT_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value().substr(2), nullptr, 8));
   case blaise_token_type::INTEGER_BASE_SIXTEEN_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_integer_value>(token_literal, stoi(token_literal.value().substr(2), nullptr, 16));
   case blaise_token_type::FLOAT_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_float_value>(token_literal, stof(token_literal.value()));
   case blaise_token_type::DOUBLE_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_double_value>(token_literal, stod(token_literal.value()));
   case blaise_token_type::CHAR_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_char_value>(token_literal, token_literal.value()[1]);
   case blaise_token_type::STRING_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_string_value>(token_literal, token_literal.value().substr(1, token_literal.value().length() - 2));
   case blaise_token_type::BOOLEAN_LITERAL:
      return memory::make_shared<ast::blaise_ast_expression_boolean_value>(token_literal, token_literal.value() == "true");
   default:
      gasp::blaise::blaise_parser::throw_parse_error_with_details(context, token_literal.line(), token_literal.column(), sanelli::make_string("Cannot extract literal from token '", token_literal, "'"));
   }
}

// NOTE: I have a specific parser for boolean becaus ein the future I would like to add
//       the triboolean type (true, false, undefined)
shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_boolean(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_boolean" << std::endl);

   shared_ptr<ast::blaise_ast_expression> boolean_literal = nullptr;

   auto literal_token = context.peek_token();
   auto token_type = literal_token.type();
   if (!blaise_token_type_utility::is_boolean(token_type))
      throw_parse_error_with_details(context, literal_token.line(), literal_token.column(), sanelli::make_string("A boolean was expected but found '", token_type, "'"));

   boolean_literal = make_blaise_ast_expression_value(context, literal_token);
   match_token(context, token_type);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_boolean" << std::endl);
   return boolean_literal;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_cast_expression(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_cast_expression" << std::endl);

   auto reference = context.peek_token();
   match_token(context, blaise_token_type::CAST);
   match_token(context, blaise_token_type::LESS_THAN);
   auto return_type = parse_variable_type(context, true);
   match_token(context, blaise_token_type::GREAT_THAN);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   auto expression = parse_expression(context);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);
   if (!ast::blaise_ast_utility::can_force_cast(expression->result_type(), return_type))
      throw_parse_error_with_details(context, reference.line(), reference.column(), sanelli::make_string("It is not possible force a cast from '", expression->result_type(), "' to '", return_type, "'."));
   auto cast_expression = ast::make_blaise_ast_expression_cast(reference, return_type, expression);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_cast_expression" << std::endl);
   return cast_expression;
}

shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_ternary_expression(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_ternary_expression" << std::endl);

   auto reference = context.peek_token();

   match_token(context, blaise_token_type::IF);
   auto condition_reference = context.peek_token();
   auto condition = parse_expression(context);
   match_token(context, blaise_token_type::THEN);
   auto then_expression = parse_expression(context);
   match_token(context, blaise_token_type::ELSE);
   auto else_expression = parse_expression(context);

   condition = ast::cast_to_boolean(condition_reference, condition);

   auto common_type = ast::blaise_ast_utility::get_common_type(reference, then_expression->result_type(), else_expression->result_type());

   then_expression = ast::introduce_cast_if_required(reference, common_type, then_expression);
   else_expression = ast::introduce_cast_if_required(reference, common_type, else_expression);

   auto ternary_expression = ast::make_blaise_ast_expression_ternary(reference, condition, then_expression, else_expression);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_ternary_expression" << std::endl);
   return ternary_expression;
}

std::shared_ptr<ast::blaise_ast_expression> blaise_parser::parse_new_expression(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_new_expression" << std::endl);

   auto reference = context.peek_token();

   match_token(context, blaise_token_type::NEW);
   match_token(context, blaise_token_type::LESS_THAN);
   auto type = parse_variable_type(context, true);
   if (!ast::blaise_ast_utility::is_array(type))
      throw_parse_error_with_details(context, reference.line(), reference.column(), "An array type was expected");
   auto array_type = ast::blaise_ast_utility::as_array_type(type);
   if (!array_type->is_unbounded())
      throw_parse_error_with_details(context, reference.line(), reference.column(), "An array type without bounds was expected.");
   match_token(context, blaise_token_type::GREAT_THAN);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   std::vector<std::shared_ptr<ast::blaise_ast_expression>> expressions;
   auto integer_type = ast::make_plain_type(ast::blaise_ast_system_type::INTEGER);
   do
   {
      auto expression_reference = context.peek_token();
      auto expression = parse_expression(context);
      if (!ast::blaise_ast_utility::is_integer(expression->result_type()) &&
          !ast::blaise_ast_utility::can_auto_cast(expression->result_type(), integer_type))
         throw_parse_error_with_details(context, reference.line(), reference.column(), "Expression was expected to be an integer");

      expression = ast::introduce_cast_if_required(expression_reference, integer_type, expression);

      expressions.push_back(expression);
   } while (is_token_and_match(context, blaise_token_type::COMMA));
   if (expressions.size() > 1)
      throw_parse_error_with_details(context, reference.line(), reference.column(), "Ony single dimension array are supported.");
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   auto new_expression = ast::make_blaise_ast_expression_new(reference, type, expressions);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_new_expression" << std::endl);
   return new_expression;
}