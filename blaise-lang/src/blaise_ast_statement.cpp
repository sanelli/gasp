#include <vector>
#include <memory>
#include <stdexcept>

#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/common/debug.hpp>
#include <gasp/common/string.hpp>
#include <gasp/common/memory.hpp>

using namespace std;
using namespace gasp::common;
using namespace gasp::blaise;
using namespace gasp::blaise::ast;

//
// GENERIC STATEMENT
//
blaise_ast_statement::blaise_ast_statement(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, blaise_ast_statement_type type) 
      : blaise_ast(reference), _type(type) {}

//
// COMPOUND STATEMENT
//
blaise_ast_statement_compund::blaise_ast_statement_compund(const gasp::common::token<gasp::blaise::blaise_token_type>& reference) 
   : blaise_ast_statement(reference, blaise_ast_statement_type::COMPOUND) {}
void blaise_ast_statement_compund::push_back(shared_ptr<blaise_ast_statement> statement) { _statements.push_back(statement); }
shared_ptr<blaise_ast_statement> blaise_ast_statement_compund::get_statement(unsigned int index) const { return _statements.at(index); }
unsigned int blaise_ast_statement_compund::get_statements_count() const { return _statements.size(); }
shared_ptr<ast::blaise_ast_statement_compund>gasp::blaise::ast::make_compound_statement(const gasp::common::token<gasp::blaise::blaise_token_type>& reference)
{
   return memory::gasp_make_shared<blaise_ast_statement_compund>(reference);
}

//
// ASSIGNMENT STATEMENT
//
blaise_ast_statement_assignment::blaise_ast_statement_assignment(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                              shared_ptr<blaise_ast_identifier> identifier,
                              shared_ptr<blaise_ast_expression> expression) 
                            : blaise_ast_statement(reference, blaise_ast_statement_type::ASSIGNEMENT),
                            _identifier(identifier),
                            _expression(expression)
                            { }
shared_ptr<blaise_ast_identifier> blaise_ast_statement_assignment::identifier() const { return _identifier; }
shared_ptr<blaise_ast_expression> blaise_ast_statement_assignment::expression() const { return _expression; }
shared_ptr<blaise_ast_statement> gasp::blaise::ast::make_assignement_statement(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                                                                  shared_ptr<blaise_ast_identifier> identifier,
                                                                  shared_ptr<blaise_ast_expression> expression) {
   std::shared_ptr<blaise_ast_type> variable_real_type = nullptr;
   std::shared_ptr<blaise_ast_generic_memory_location> variable = nullptr;

   switch(identifier->type()) {
      case blaise_ast_identifier_type::VARIABLE:
      {
         auto variable_identifier = std::static_pointer_cast<blaise_ast_variable_identifier>(identifier);
         variable = variable_identifier->variable();
         variable_real_type = variable->type();
         if(variable->type()->type_type() == ast::blaise_ast_type_type::ARRAY)
            throw blaise_ast_error(reference.line(), reference.column(), make_string("Unsupported type: '", variable_real_type, "'. An array should not be used here."));
      } 
      break;
      case blaise_ast_identifier_type::ARRAY:
      {
         auto array_identifier = std::static_pointer_cast<blaise_ast_array_identifier>(identifier);
         variable = array_identifier->variable();
         variable_real_type = ast::blaise_ast_utility::as_array_type(variable->type())->inner_type();
         if(variable->type()->type_type() != ast::blaise_ast_type_type::ARRAY)
            throw blaise_ast_error(reference.line(), reference.column(), make_string("Unsupported type: '", variable_real_type, "'. An array was expected here."));
      }
      break;
      default:
         throw blaise_ast_error(reference.line(), reference.column(), "Unexpected identifier type.");
   }

   switch(variable->variable_type()){
      case blaise_ast_variable_type::VARIABLE:
      case blaise_ast_variable_type::PARAMETER:
         break; // All good!
      case blaise_ast_variable_type::CONSTANT:
         throw blaise_ast_error(reference.line(), reference.column(), make_string("Variable '", variable->name() ,"' is constant."));
      default:
         throw blaise_ast_error(reference.line(), reference.column(), make_string("Unexpected variable type"));
   }

    if(expression->result_type() !=variable_real_type && !ast::blaise_ast_utility::can_auto_cast(expression->result_type(), variable_real_type))
      throw blaise_ast_error(reference.line(), reference.column(), make_string("Cannot cast '", expression->result_type(), "' into '",variable_real_type,"'."));

   if(variable_real_type == nullptr)
      throw blaise_ast_error(reference.line(), reference.column(), "Variable type or underlying type cannot be detected.");

   expression = ast::introduce_cast_if_required(reference, variable_real_type, expression);
   
   return memory::gasp_make_shared<blaise_ast_statement_assignment>(reference, identifier, expression);
}

//
// SUBROUTINE CALL
//
   blaise_ast_statement_subroutine_call::blaise_ast_statement_subroutine_call(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                  std::shared_ptr<blaise_ast_subroutine> subroutine,
                  const std::vector<std::shared_ptr<blaise_ast_expression>>& parameters) : 
            blaise_ast_statement(reference, blaise_ast_statement_type::SUBROUTINE_CALL),
            _subroutine(subroutine),
            _actual_parameters(parameters)
            {}
   std::shared_ptr<blaise_ast_subroutine> blaise_ast_statement_subroutine_call::subroutine() const { return _subroutine;}
   unsigned int blaise_ast_statement_subroutine_call::parameters_count() const { return _actual_parameters.size(); }
   std::shared_ptr<blaise_ast_expression> blaise_ast_statement_subroutine_call::actual_parameter(unsigned int index) const { return _actual_parameters.at(index); }

std::shared_ptr<blaise_ast_statement> gasp::blaise::ast::make_blaise_ast_statement_subroutine_call(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
         std::shared_ptr<blaise_ast_subroutine> subroutine,
         const std::vector<std::shared_ptr<blaise_ast_expression>>& parameters) {
   return memory::gasp_make_shared<blaise_ast_statement_subroutine_call>(reference, subroutine, parameters);
}

//
// IF STATEMENT
//
blaise_ast_statement_if::blaise_ast_statement_if(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> then_statement,
      std::shared_ptr<blaise_ast_statement> else_statement) 
      : blaise_ast_statement(reference, blaise_ast_statement_type::IF),
         _condition(condition),
         _then_statement(then_statement),
         _else_statement(else_statement)
{  
   if(condition->result_type() != ast::make_plain_type(ast::blaise_ast_system_type::BOOLEAN)){
        throw blaise_ast_error(reference.line(), reference.column(), make_string("Condition must be a boolean expression"));
     }
}
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_if::condition() const { return _condition; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_if::then_statement() const { return _then_statement; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_if::else_statement() const { return _else_statement; }
std::shared_ptr<blaise_ast_statement_if> ast::make_blaise_ast_statement_if(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> then_statement,
      std::shared_ptr<blaise_ast_statement> else_statement) {
   return memory::gasp_make_shared<blaise_ast_statement_if>(reference, condition, then_statement, else_statement);
}

//
// FOR LOOP
//
blaise_ast_statement_for_loop::blaise_ast_statement_for_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_identifier> variable,
      std::shared_ptr<blaise_ast_expression> from_expression,
      std::shared_ptr<blaise_ast_expression> to_expression,
      std::shared_ptr<blaise_ast_expression> step_expression,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally)
      : blaise_ast_statement(reference, blaise_ast_statement_type::FOR_LOOP),
        _variable(variable),
        _from_expression(from_expression),
        _to_expression(to_expression),
        _step_expression(step_expression),
        _body(body),
        _finally(finally)
      { }
std::shared_ptr<blaise_ast_identifier> blaise_ast_statement_for_loop::variable() const { return _variable; }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_for_loop::from_expression() const { return _from_expression; }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_for_loop::to_expression() const { return _to_expression; }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_for_loop::step_expression() const { return _step_expression; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_for_loop::body() const { return _body; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_for_loop::finally() const { return _finally; }
std::shared_ptr<blaise_ast_statement_for_loop> ast::make_blaise_ast_statement_for_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_identifier> variable,
      std::shared_ptr<blaise_ast_expression> from_expression,
      std::shared_ptr<blaise_ast_expression> to_expression,
      std::shared_ptr<blaise_ast_expression> step_expression,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally){
   return memory::gasp_make_shared<blaise_ast_statement_for_loop>(reference, variable, from_expression, to_expression, step_expression, body, finally);
}

//
// GENERIC LOOP
//
blaise_ast_statement_generic_loop::blaise_ast_statement_generic_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      blaise_ast_statement_type type,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally)
      : blaise_ast_statement(reference, type),
       _condition(condition),
       _body(body),
       _finally(finally)
      {
      }
std::shared_ptr<blaise_ast_expression> blaise_ast_statement_generic_loop::condition() const { return _condition; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_generic_loop::body() const { return _body; }
std::shared_ptr<blaise_ast_statement> blaise_ast_statement_generic_loop::finally() const { return _finally; }

//
// DO-WHILE LOOP
//
blaise_ast_statement_dowhile_loop::blaise_ast_statement_dowhile_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally)
      : blaise_ast_statement_generic_loop(reference, blaise_ast_statement_type::DO_WHILE_LOOP, condition, body, finally)
   {}
std::shared_ptr<blaise_ast_statement_dowhile_loop> ast::make_blaise_ast_statement_dowhile_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally){
   return memory::gasp_make_shared<blaise_ast_statement_dowhile_loop>(reference, condition, body, finally);
}

//
// WHILE LOOP
//
blaise_ast_statement_while_loop::blaise_ast_statement_while_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally)
      : blaise_ast_statement_generic_loop(reference, blaise_ast_statement_type::WHILE_LOOP, condition, body, finally)
   {}
std::shared_ptr<blaise_ast_statement_while_loop> ast::make_blaise_ast_statement_while_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally){
   return memory::gasp_make_shared<blaise_ast_statement_while_loop>(reference, condition, body, finally);
}

//
// REPEAT UNTIL LOOP
//
blaise_ast_statement_repeatuntil_loop::blaise_ast_statement_repeatuntil_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally)
      : blaise_ast_statement_generic_loop(reference, blaise_ast_statement_type::REPEAT_UNTIL_LOOP, condition, body, finally)
   {}
std::shared_ptr<blaise_ast_statement_repeatuntil_loop> ast::make_blaise_ast_statement_repeatuntil_loop(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
      std::shared_ptr<blaise_ast_expression> condition,
      std::shared_ptr<blaise_ast_statement> body,
      std::shared_ptr<blaise_ast_statement> finally){
   return memory::gasp_make_shared<blaise_ast_statement_repeatuntil_loop>(reference, condition, body, finally);
}