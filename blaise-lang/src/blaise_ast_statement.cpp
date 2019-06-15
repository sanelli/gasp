#include <vector>
#include <memory>


#include <gasp/common/tokenizer.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/blaise/ast.hpp>
#include <gasp/common/debug.hpp>
#include <gasp/common/string.hpp>

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
   return shared_ptr<blaise_ast_statement_compund>(new blaise_ast_statement_compund(reference));
}

//
// ASSIGNMENT STATEMENT
//
blaise_ast_statement_assignment::blaise_ast_statement_assignment(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                              shared_ptr<blaise_ast_generic_memory_location> variable,
                              shared_ptr<blaise_ast_expression> expression) 
                            : blaise_ast_statement(reference, blaise_ast_statement_type::ASSIGNEMENT),
                            _variable(variable),
                            _expression(expression)
                            { }
shared_ptr<blaise_ast_generic_memory_location> blaise_ast_statement_assignment::variable() const { return _variable; }
shared_ptr<blaise_ast_expression> blaise_ast_statement_assignment::expression() const { return _expression; }
shared_ptr<blaise_ast_statement> gasp::blaise::ast::make_assignement_statement(const gasp::common::token<gasp::blaise::blaise_token_type>& reference,
                                                                  shared_ptr<blaise_ast_generic_memory_location> variable,
                                                                  shared_ptr<blaise_ast_expression> expression) {
   if(variable->variable_type() == blaise_ast_variable_type::CONSTANT)
      throw blaise_ast_error(reference.line(), reference.column(), make_string("Variable '", variable->name() ,"' is constant."));

   if(expression->result_type() != variable->type() && !ast::blaise_ast_utility::can_auto_cast(expression->result_type(), variable->type())){
        throw blaise_ast_error(reference.line(), reference.column(), make_string("Cannot cast '", expression->result_type(), "' into '",variable->type(),"'."));
     }

   expression = ast::introduce_cast_if_required(reference, variable->type(), expression);
   
   return shared_ptr<blaise_ast_statement_assignment>(new blaise_ast_statement_assignment(reference, variable, expression));
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
            return shared_ptr<blaise_ast_statement_subroutine_call>(new blaise_ast_statement_subroutine_call(reference, subroutine, parameters));
}