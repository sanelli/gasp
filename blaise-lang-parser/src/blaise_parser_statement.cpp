#include <memory>
#include <vector>
#include <string>

#include <sanelli/sanelli.hpp>

#include <gasp/blaise/parser/parser.hpp>
#include <gasp/blaise/ast.hpp>

using namespace std;
using namespace gasp::blaise;

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_statement(blaise_parser_context &context,
                                                                          std::shared_ptr<ast::blaise_ast_statement_compund> parent)
{
   std::shared_ptr<ast::blaise_ast_statement> statement = nullptr;

   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_statement" << std::endl);

   const auto token = context.peek_token();
   const auto token_type = token.type();
   auto match_ending_semicolon = true;

   switch (token_type)
   {
   case blaise_token_type::IDENTIFIER:
   {
      const auto lookahead = context.peek_token(1);
      const auto lookahead_type = lookahead.type();

      switch (lookahead_type)
      {
      case blaise_token_type::LEFT_PARENTHESES:
         statement = parse_subroutine_call_statement(context);
         break;
      case blaise_token_type::ASSIGNMENT:   // Variable assignment
      case blaise_token_type::LEFT_BRACKET: // Array assignement
         statement = parse_assignamet_statement(context);
         break;
      default:
         throw_parse_error_with_details(context, token.line(), token.column(), sanelli::make_string("Unexpected token '", token_type, "' found after indetifier."));
      }
   }
   break;
   case blaise_token_type::BEGIN:
      statement = parse_compound_statement(context, parent);
      match_ending_semicolon = false;
      break;
   case blaise_token_type::IF:
      statement = parse_if_statement(context);
      match_ending_semicolon = false;
      break;
   case blaise_token_type::FOR:
      statement = parse_for_loop_statement(context);
      match_ending_semicolon = false;
      break;
   case blaise_token_type::DO:
      statement = parse_do_while_loop_statement(context);
      break;
   case blaise_token_type::WHILE:
      statement = parse_while_loop_statement(context);
      match_ending_semicolon = false;
      break;
   case blaise_token_type::REPEAT:
      statement = parse_repeat_until_loop_statement(context);
      break;
   case blaise_token_type::SEMICOLON:
      statement = parse_empty_statement(context);
      match_ending_semicolon = false;
      break;
   case blaise_token_type::DELETE:
      statement = parse_delete_statement(context);
      break;
   default:
      throw_parse_error_with_details(context, token.line(), token.column(), sanelli::make_string("Unexpected token '", token_type, "' found."));
   }
   if (match_ending_semicolon)
      match_token(context, blaise_token_type::SEMICOLON);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_statement" << std::endl);

   return statement;
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_compound_statement(blaise_parser_context &context, std::shared_ptr<ast::blaise_ast_statement_compund> parent)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_compound_statement" << std::endl);

   auto begin_token = context.peek_token();
   match_token(context, blaise_token_type::BEGIN);
   auto compund_statement = ast::make_compound_statement(begin_token);
   if (parent != nullptr)
      parent->push_back(compund_statement);
   while (!is_token(context, blaise_token_type::END))
   {
      auto statement = parse_statement(context);
      compund_statement->push_back(statement);
   }
   match_token(context, blaise_token_type::END);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_compound_statement" << std::endl);
   return compund_statement;
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_subroutine_call_statement(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_call_statement" << std::endl);

   auto identifier_token = context.peek_token();
   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   vector<shared_ptr<ast::blaise_ast_expression>> expressions;
   vector<std::shared_ptr<ast::blaise_ast_type>> types;
   parse_subroutine_call_parameters(context, expressions, types);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);

   auto subroutine = context.module()->get_subroutine(identifier_token, types);
   if (subroutine == nullptr)
      throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(), sanelli::make_string("Cannot find function '", identifier_token.value(), "(", types, ")'"));

   ast::introduce_cast_if_required(identifier_token, subroutine, expressions);

   auto statement = ast::make_blaise_ast_statement_subroutine_call(identifier_token, subroutine, expressions);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_subroutine_call_statement" << std::endl);

   return statement;
}

void blaise_parser::parse_subroutine_call_parameters(blaise_parser_context &context,
                                                     std::vector<std::shared_ptr<ast::blaise_ast_expression>> &expressions,
                                                     std::vector<std::shared_ptr<ast::blaise_ast_type>> &types)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_subroutine_call_parameters" << std::endl);

   auto token_type = context.peek_token().type();

   if (token_type != blaise_token_type::RIGHT_PARENTHESES)
      do
      {
         auto expression = parse_expression(context);
         expressions.push_back(expression);
         types.push_back(expression->result_type());

         if (is_token_and_match(context, blaise_token_type::COMMA))
            continue;
         else if (!is_token(context, blaise_token_type::RIGHT_PARENTHESES))
         {
            auto token = context.peek_token();
            throw_parse_error_with_details(context, token.line(), token.column(), "expected ')' or ','.");
         }
         token_type = context.peek_token().type();
      } while (token_type != blaise_token_type::RIGHT_PARENTHESES);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_subroutine_call_parameters" << std::endl);
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_assignamet_statement(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_assignment" << std::endl);

   auto identifier = context.peek_token();
   match_token(context, blaise_token_type::IDENTIFIER);
   auto variable = context.current_subroutine()->get_memory_location(identifier.value());

   if (variable == nullptr)
      throw_parse_error_with_details(context, identifier.line(), identifier.column(), sanelli::make_string("Variable ", identifier.value(), " does not exists in the context."));

   if (variable->variable_type() == ast::blaise_ast_variable_type::CONSTANT)
      throw_parse_error_with_details(context, identifier.line(), identifier.column(),
                                     sanelli::make_string("Constant '", identifier.value(), "' cannot be assigned"));

   std::shared_ptr<ast::blaise_ast_identifier> variable_identifier = nullptr;

   if (is_token_and_match(context, blaise_token_type::LEFT_BRACKET))
   { // Assignment to array
      std::vector<std::shared_ptr<ast::blaise_ast_expression>> indexing_expressions;
      do {
         auto index_token = context.peek_token();
         auto indexing_expression = parse_expression(context);
         auto integer_type = ast::make_plain_type(ast::blaise_ast_system_type::INTEGER);

         if (!indexing_expression->result_type()->equals(integer_type) && 
               !ast::blaise_ast_utility::can_auto_cast(indexing_expression->result_type(), integer_type))
         {
            throw_parse_error_with_details(context, indexing_expression->line(), indexing_expression->column(),
                                          "Indexing expression must be an integer or a type that can be casted to an integer.");
         }
         indexing_expression = ast::introduce_cast_if_required(index_token, integer_type, indexing_expression);
         indexing_expressions.push_back(indexing_expression);
         is_token_and_match(context, blaise_token_type::COMMA);
      } while(!is_token_and_match(context, blaise_token_type::RIGHT_BRACKET));
     
      auto array_type = ast::blaise_ast_utility::as_array_type(variable->type());
      if(array_type->dimensions() != indexing_expressions.size())
             throw_parse_error_with_details(context, identifier.line(), identifier.column(),
                                             sanelli::make_string("Number of dimensions (",indexing_expressions.size(),") while accessing array \"",
                                                            variable->name(),
                                                            "\" does not match type number of dimensions (",array_type->dimensions(),")"));
   
      variable_identifier = ast::make_blaise_ast_array_identifier(identifier, variable, indexing_expressions);
   }
   else
   { // Standard variable identifier
      variable_identifier = ast::make_blaise_ast_variable_identifier(identifier, variable);
   }

   match_token(context, blaise_token_type::ASSIGNMENT);
   auto expression = parse_expression(context);

   auto statement = ast::make_assignement_statement(identifier, variable_identifier, expression);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_assignment" << std::endl);

   return statement;
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_if_statement(blaise_parser_context &context)
{
   SANELLI_DEBUG("blaise-parser", "[ENTER] blaise_parser::parse_if_statement" << std::endl);

   auto reference = context.peek_token();
   match_token(context, blaise_token_type::IF);
   auto condition_reference = context.peek_token();
   auto condition_expression = parse_expression(context);
   match_token(context, blaise_token_type::THEN);
   auto then_statement = parse_statement(context);
   std::shared_ptr<ast::blaise_ast_statement> else_statement = nullptr;
   if (is_token_and_match(context, blaise_token_type::ELSE))
      else_statement = parse_statement(context);

   condition_expression = ast::cast_to_boolean(condition_reference, condition_expression);

   auto if_statement = ast::make_blaise_ast_statement_if(reference, condition_expression, then_statement, else_statement);

   SANELLI_DEBUG("blaise-parser", "[EXIT] blaise_parser::parse_if_statement" << std::endl);
   return if_statement;
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_for_loop_statement(blaise_parser_context &context)
{
   auto integer_type = ast::make_plain_type(ast::blaise_ast_system_type::INTEGER);

   auto loop_token = context.peek_token();
   match_token(context, blaise_token_type::FOR);
   auto identifier_token = context.peek_token();
   match_token(context, blaise_token_type::IDENTIFIER);
   auto variable = context.current_subroutine()->get_memory_location(identifier_token.value());
   if (variable == nullptr)
      throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                     sanelli::make_string("Cannot find variable with name '", identifier_token.value(), "'"));
   if (variable->variable_type() == ast::blaise_ast_variable_type::CONSTANT)
      throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                     sanelli::make_string("Cannot user constant '", identifier_token.value(), "' as for loop indexing"));
   if (!ast::blaise_ast_utility::is_integer(variable->type()))
      throw_parse_error_with_details(context, identifier_token.line(), identifier_token.column(),
                                     sanelli::make_string("Cannot user variable '", identifier_token.value(), "': it must be of type 'integer'."));
   auto variable_identifier = ast::make_blaise_ast_variable_identifier(identifier_token, variable);

   match_token(context, blaise_token_type::FROM);
   auto from_expression = parse_expression(context);
   if (!ast::blaise_ast_utility::is_integer(from_expression->result_type()) && !ast::blaise_ast_utility::can_auto_cast(from_expression->result_type(), integer_type))
      throw_parse_error_with_details(context, from_expression->line(), from_expression->column(),
                                     "FROM expression must be integer.");
   from_expression = ast::introduce_cast_if_required(loop_token, integer_type, from_expression);

   match_token(context, blaise_token_type::TO);
   auto to_expression = parse_expression(context);
   if (!ast::blaise_ast_utility::is_integer(to_expression->result_type()) && !ast::blaise_ast_utility::can_auto_cast(to_expression->result_type(), integer_type))
      throw_parse_error_with_details(context, to_expression->line(), to_expression->column(),
                                     "TO expression must be integer.");
   to_expression = ast::introduce_cast_if_required(loop_token, integer_type, to_expression);

   std::shared_ptr<ast::blaise_ast_expression> step_expression;
   if (is_token_and_match(context, blaise_token_type::STEP))
   {
      step_expression = parse_expression(context);
      if (!ast::blaise_ast_utility::is_integer(step_expression->result_type()) && !ast::blaise_ast_utility::can_auto_cast(step_expression->result_type(), integer_type))
         throw_parse_error_with_details(context, step_expression->line(), step_expression->column(),
                                        "STEP expression must be integer.");
      step_expression = ast::introduce_cast_if_required(loop_token, integer_type, step_expression);
   }
   auto body = parse_statement(context);

   return ast::make_blaise_ast_statement_for_loop(loop_token, variable_identifier, from_expression, to_expression, step_expression, body);
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_while_loop_statement(blaise_parser_context &context)
{
   auto boolean_type = ast::make_plain_type(ast::blaise_ast_system_type::BOOLEAN);
   auto loop_token = context.peek_token();
   match_token(context, blaise_token_type::WHILE);
   auto condition = parse_expression(context);
   if (!ast::blaise_ast_utility::is_boolean(condition->result_type()) && !ast::blaise_ast_utility::can_auto_cast(condition->result_type(), boolean_type))
      throw_parse_error_with_details(context, condition->line(), condition->column(),
                                     "Condition must be a boolean expression.");
   condition = ast::introduce_cast_if_required(loop_token, boolean_type, condition);
   auto body = parse_statement(context);
   return ast::make_blaise_ast_statement_while_loop(loop_token, condition, body);
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_do_while_loop_statement(blaise_parser_context &context)
{
   auto boolean_type = ast::make_plain_type(ast::blaise_ast_system_type::BOOLEAN);
   auto loop_token = context.peek_token();
   match_token(context, blaise_token_type::DO);
   auto body = parse_statement(context);
   match_token(context, blaise_token_type::WHILE);
   auto condition = parse_expression(context);
   if (!ast::blaise_ast_utility::is_boolean(condition->result_type()) && !ast::blaise_ast_utility::can_auto_cast(condition->result_type(), boolean_type))
      throw_parse_error_with_details(context, condition->line(), condition->column(),
                                     "Condition must be a boolean expression.");
   condition = ast::introduce_cast_if_required(loop_token, boolean_type, condition);
   return ast::make_blaise_ast_statement_dowhile_loop(loop_token, condition, body);
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_repeat_until_loop_statement(blaise_parser_context &context)
{
   auto boolean_type = ast::make_plain_type(ast::blaise_ast_system_type::BOOLEAN);
   auto loop_token = context.peek_token();
   match_token(context, blaise_token_type::REPEAT);
   auto body = parse_statement(context);
   match_token(context, blaise_token_type::UNTIL);
   auto condition = parse_expression(context);
   if (!ast::blaise_ast_utility::is_boolean(condition->result_type()) && !ast::blaise_ast_utility::can_auto_cast(condition->result_type(), boolean_type))
      throw_parse_error_with_details(context, condition->line(), condition->column(),
                                     "Condition must be a boolean expression.");
   condition = ast::introduce_cast_if_required(loop_token, boolean_type, condition);
   return ast::make_blaise_ast_statement_repeatuntil_loop(loop_token, condition, body);
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_empty_statement(blaise_parser_context &context)
{
   auto token = context.peek_token();
   match_token(context, blaise_token_type::SEMICOLON);
   return ast::make_empty_statement(token);
}

std::shared_ptr<ast::blaise_ast_statement> blaise_parser::parse_delete_statement(blaise_parser_context &context)
{
   auto token = context.peek_token();
   match_token(context, blaise_token_type::DELETE);
   match_token(context, blaise_token_type::LEFT_PARENTHESES);
   auto identifier_token = context.peek_token();
   auto variable = context.current_subroutine()->get_memory_location(identifier_token.value());
   auto variable_identifier = ast::make_blaise_ast_variable_identifier(identifier_token, variable);

   if (variable->variable_type() == ast::blaise_ast_variable_type::CONSTANT)
      throw_parse_error_with_details(context, token.line(), token.column(),
                                     "Delete operator cannot be applied to constants.");

   if (!ast::blaise_ast_utility::is_array(variable->type()))
      throw_parse_error_with_details(context, token.line(), token.column(),
                                     sanelli::make_string("Delete operator cannot be applied to variable of type ", ast::to_string(variable->type()), "."));

   match_token(context, blaise_token_type::IDENTIFIER);
   match_token(context, blaise_token_type::RIGHT_PARENTHESES);
   return ast::make_delete_statement(token, variable_identifier);
}