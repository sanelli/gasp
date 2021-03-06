#include <vector>
#include <memory>
#include <stdexcept>

#include <gasp/blaise/ast.hpp>
#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace std;

using namespace gasp::blaise;
using namespace gasp::blaise::ast;

//
// GENERIC STATEMENT
//
blaise_ast_statement::blaise_ast_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference, blaise_ast_statement_type type)
    : blaise_ast(reference), _type(type) {}
blaise_ast_statement_type blaise_ast_statement::type() { return _type; }

//
// EMPTY STATEMENT
//
blaise_ast_statement_empty::blaise_ast_statement_empty(const sanelli::token<gasp::blaise::blaise_token_type> &reference)
    : blaise_ast_statement(reference, blaise_ast_statement_type::EMPTY) {}
shared_ptr<ast::blaise_ast_statement_empty> gasp::blaise::ast::make_empty_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference)
{
   return memory::make_shared<blaise_ast_statement_empty>(reference);
}

//
// COMPOUND STATEMENT
//
blaise_ast_statement_compund::blaise_ast_statement_compund(const sanelli::token<gasp::blaise::blaise_token_type> &reference)
    : blaise_ast_statement(reference, blaise_ast_statement_type::COMPOUND) {}
void blaise_ast_statement_compund::push_back(shared_ptr<blaise_ast_statement> statement) { _statements.push_back(statement); }
shared_ptr<blaise_ast_statement> blaise_ast_statement_compund::get_statement(unsigned int index) const { return _statements.at(index); }
unsigned int blaise_ast_statement_compund::get_statements_count() const { return _statements.size(); }
shared_ptr<ast::blaise_ast_statement_compund> gasp::blaise::ast::make_compound_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference)
{
   return memory::make_shared<blaise_ast_statement_compund>(reference);
}

//
// ASSIGNMENT STATEMENT
//
blaise_ast_statement_assignment::blaise_ast_statement_assignment(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                 shared_ptr<blaise_ast_identifier> identifier,
                                                                 shared_ptr<blaise_ast_expression> expression)
    : blaise_ast_statement(reference, blaise_ast_statement_type::ASSIGNEMENT),
      _identifier(identifier),
      _expression(expression)
{
}
shared_ptr<blaise_ast_identifier> blaise_ast_statement_assignment::identifier() const { return _identifier; }
shared_ptr<blaise_ast_expression> blaise_ast_statement_assignment::expression() const { return _expression; }
shared_ptr<blaise_ast_statement> gasp::blaise::ast::make_assignement_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                               shared_ptr<blaise_ast_identifier> identifier,
                                                                               shared_ptr<blaise_ast_expression> expression)
{
   std::shared_ptr<blaise_ast_type> variable_real_type = nullptr;
   std::shared_ptr<blaise_ast_generic_memory_location> variable = nullptr;

   switch (identifier->type())
   {
   case blaise_ast_identifier_type::VARIABLE:
   {
      auto variable_identifier = std::static_pointer_cast<blaise_ast_variable_identifier>(identifier);
      variable = variable_identifier->variable();
      variable_real_type = variable->type();
   }
   break;
   case blaise_ast_identifier_type::ARRAY:
   {
      auto array_identifier = std::static_pointer_cast<blaise_ast_array_identifier>(identifier);
      variable = array_identifier->variable();
      variable_real_type = ast::blaise_ast_utility::as_array_type(variable->type())->underlying_type();
      if (variable->type()->type_type() != ast::blaise_ast_type_type::ARRAY)
         throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Unsupported type: '", to_string(variable_real_type), "'. An array was expected here."));
   }
   break;
   default:
      throw blaise_ast_error(reference.line(), reference.column(), "Unexpected identifier type.");
   }

   switch (variable->variable_type())
   {
   case blaise_ast_variable_type::VARIABLE:
   case blaise_ast_variable_type::PARAMETER:
      break; // All good!
   case blaise_ast_variable_type::CONSTANT:
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Variable '", variable->name(), "' is constant."));
   default:
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Unexpected variable type"));
   }

   if (identifier->type() == blaise_ast_identifier_type::VARIABLE && ast::blaise_ast_utility::is_array(std::static_pointer_cast<blaise_ast_variable_identifier>(identifier)->variable()->type()) && ast::blaise_ast_utility::is_array(expression->result_type()))
   {
      auto target_type = ast::blaise_ast_utility::as_array_type(std::static_pointer_cast<blaise_ast_variable_identifier>(identifier)->variable()->type());
      auto expression_type = ast::blaise_ast_utility::as_array_type(expression->result_type());

      auto valid_assign = target_type->equals(expression_type) || (target_type->dimensions() == expression_type->dimensions() && target_type->is_unbounded() && target_type->underlying_type()->equals(expression_type->underlying_type()));

      if (!valid_assign)
         throw blaise_ast_error(reference.line(), reference.column(), 
            sanelli::make_string("Cannot assign expression of type '", ast::to_string(expression_type), 
            "' to a bounded array '", variable->name(), "' of type '", ast::to_string(target_type), "'."));
   }
   else
   {
      if (!expression->result_type()->equals(variable_real_type) &&
          !ast::blaise_ast_utility::is_array(expression->result_type()) &&
          !ast::blaise_ast_utility::can_auto_cast(expression->result_type(), variable_real_type))
         throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Cannot cast '", to_string(expression->result_type()), "' into '", to_string(variable_real_type), "'."));

      if (variable_real_type == nullptr)
         throw blaise_ast_error(reference.line(), reference.column(), "Variable type or underlying type cannot be detected.");

      expression = ast::introduce_cast_if_required(reference, variable_real_type, expression);
   }

   return memory::make_shared<blaise_ast_statement_assignment>(reference, identifier, expression);
}

//
// SUBROUTINE CALL
//
blaise_ast_statement_subroutine_call::blaise_ast_statement_subroutine_call(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                           std::shared_ptr<blaise_ast_subroutine> subroutine,
                                                                           const std::vector<std::shared_ptr<blaise_ast_expression>> &parameters) : blaise_ast_statement(reference, blaise_ast_statement_type::SUBROUTINE_CALL),
                                                                                                                                                    _subroutine(subroutine),
                                                                                                                                                    _actual_parameters(parameters)
{
}
std::shared_ptr<blaise_ast_subroutine> blaise_ast_statement_subroutine_call::subroutine() const { return _subroutine; }
unsigned int blaise_ast_statement_subroutine_call::parameters_count() const { return _actual_parameters.size(); }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_subroutine_call::actual_parameter(unsigned int index) const { return _actual_parameters.at(index); }

std::shared_ptr<blaise_ast_statement> gasp::blaise::ast::make_blaise_ast_statement_subroutine_call(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                                   std::shared_ptr<blaise_ast_subroutine> subroutine,
                                                                                                   const std::vector<std::shared_ptr<blaise_ast_expression>> &parameters)
{
   return memory::make_shared<blaise_ast_statement_subroutine_call>(reference, subroutine, parameters);
}

//
// IF STATEMENT
//
blaise_ast_statement_if::blaise_ast_statement_if(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                 std::shared_ptr<blaise_ast_expression> condition,
                                                 std::shared_ptr<blaise_ast_statement> then_statement,
                                                 std::shared_ptr<blaise_ast_statement> else_statement)
    : blaise_ast_statement(reference, blaise_ast_statement_type::IF),
      _condition(condition),
      _then_statement(then_statement),
      _else_statement(else_statement)
{
   if (condition->result_type() != ast::make_plain_type(ast::blaise_ast_system_type::BOOLEAN))
   {
      throw blaise_ast_error(reference.line(), reference.column(), sanelli::make_string("Condition must be a boolean expression"));
   }
}
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_if::condition() const { return _condition; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_if::then_statement() const { return _then_statement; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_if::else_statement() const { return _else_statement; }
std::shared_ptr<blaise_ast_statement_if> ast::make_blaise_ast_statement_if(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                           std::shared_ptr<blaise_ast_expression> condition,
                                                                           std::shared_ptr<blaise_ast_statement> then_statement,
                                                                           std::shared_ptr<blaise_ast_statement> else_statement)
{
   return memory::make_shared<blaise_ast_statement_if>(reference, condition, then_statement, else_statement);
}

//
// FOR LOOP
//
blaise_ast_statement_for_loop::blaise_ast_statement_for_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                             std::shared_ptr<blaise_ast_identifier> variable,
                                                             std::shared_ptr<blaise_ast_expression> from_expression,
                                                             std::shared_ptr<blaise_ast_expression> to_expression,
                                                             std::shared_ptr<blaise_ast_expression> step_expression,
                                                             std::shared_ptr<blaise_ast_statement> body)
    : blaise_ast_statement(reference, blaise_ast_statement_type::FOR_LOOP),
      _variable(variable),
      _from_expression(from_expression),
      _to_expression(to_expression),
      _step_expression(step_expression),
      _body(body)
{
}
std::shared_ptr<blaise_ast_identifier> blaise_ast_statement_for_loop::variable() const { return _variable; }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_for_loop::from_expression() const { return _from_expression; }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_for_loop::to_expression() const { return _to_expression; }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_for_loop::step_expression() const { return _step_expression; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_for_loop::body() const { return _body; }
std::shared_ptr<blaise_ast_statement_for_loop> ast::make_blaise_ast_statement_for_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                       std::shared_ptr<blaise_ast_identifier> variable,
                                                                                       std::shared_ptr<blaise_ast_expression> from_expression,
                                                                                       std::shared_ptr<blaise_ast_expression> to_expression,
                                                                                       std::shared_ptr<blaise_ast_expression> step_expression,
                                                                                       std::shared_ptr<blaise_ast_statement> body)
{
   return memory::make_shared<blaise_ast_statement_for_loop>(reference, variable, from_expression, to_expression, step_expression, body);
}

//
// GENERIC LOOP
//
blaise_ast_statement_generic_loop::blaise_ast_statement_generic_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                     blaise_ast_statement_type type,
                                                                     std::shared_ptr<blaise_ast_expression> condition,
                                                                     std::shared_ptr<blaise_ast_statement> body)
    : blaise_ast_statement(reference, type),
      _condition(condition),
      _body(body)
{
}
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_generic_loop::condition() const { return _condition; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_generic_loop::body() const { return _body; }

//
// DO-WHILE LOOP
//
blaise_ast_statement_dowhile_loop::blaise_ast_statement_dowhile_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                     std::shared_ptr<blaise_ast_expression> condition,
                                                                     std::shared_ptr<blaise_ast_statement> body)
    : blaise_ast_statement_generic_loop(reference, blaise_ast_statement_type::DO_WHILE_LOOP, condition, body)
{
}
std::shared_ptr<blaise_ast_statement_dowhile_loop> ast::make_blaise_ast_statement_dowhile_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                               std::shared_ptr<blaise_ast_expression> condition,
                                                                                               std::shared_ptr<blaise_ast_statement> body)
{
   return memory::make_shared<blaise_ast_statement_dowhile_loop>(reference, condition, body);
}

//
// WHILE LOOP
//
blaise_ast_statement_while_loop::blaise_ast_statement_while_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                 std::shared_ptr<blaise_ast_expression> condition,
                                                                 std::shared_ptr<blaise_ast_statement> body)
    : blaise_ast_statement_generic_loop(reference, blaise_ast_statement_type::WHILE_LOOP, condition, body)
{
}
std::shared_ptr<blaise_ast_statement_while_loop> ast::make_blaise_ast_statement_while_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                           std::shared_ptr<blaise_ast_expression> condition,
                                                                                           std::shared_ptr<blaise_ast_statement> body)
{
   return memory::make_shared<blaise_ast_statement_while_loop>(reference, condition, body);
}

//
// REPEAT UNTIL LOOP
//
blaise_ast_statement_repeatuntil_loop::blaise_ast_statement_repeatuntil_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                             std::shared_ptr<blaise_ast_expression> condition,
                                                                             std::shared_ptr<blaise_ast_statement> body)
    : blaise_ast_statement_generic_loop(reference, blaise_ast_statement_type::REPEAT_UNTIL_LOOP, condition, body)
{
}
std::shared_ptr<blaise_ast_statement_repeatuntil_loop> ast::make_blaise_ast_statement_repeatuntil_loop(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                                                       std::shared_ptr<blaise_ast_expression> condition,
                                                                                                       std::shared_ptr<blaise_ast_statement> body)
{
   return memory::make_shared<blaise_ast_statement_repeatuntil_loop>(reference, condition, body);
}

// DELETE

blaise_ast_statement_delete::blaise_ast_statement_delete(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                         std::shared_ptr<blaise_ast_identifier> identifier)
    : blaise_ast_statement(reference, blaise_ast_statement_type::DELETE),
      _identifier(identifier)
{
}

std::shared_ptr<blaise_ast_identifier> blaise_ast_statement_delete::identifier() const { return _identifier; }

std::shared_ptr<blaise_ast_statement_delete> ast::make_delete_statement(const sanelli::token<gasp::blaise::blaise_token_type> &reference,
                                                                        std::shared_ptr<blaise_ast_identifier> identifier)
{
   return sanelli::memory::make_shared<blaise_ast_statement_delete>(reference, identifier);
}
