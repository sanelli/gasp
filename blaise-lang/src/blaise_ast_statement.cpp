#include <vector>
#include <memory>

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
blaise_statement::blaise_statement(blaise_statement_type type) : _type(type) {}

//
// COMPOUND STATEMENT
//
blaise_statement_compund::blaise_statement_compund() : blaise_statement(blaise_statement_type::COMPOUND) {}
void blaise_statement_compund::push_back(shared_ptr<blaise_statement> statement) { _statements.push_back(statement); }
shared_ptr<blaise_statement> blaise_statement_compund::get_statement(unsigned int index) const { return _statements.at(index); }
unsigned int blaise_statement_compund::get_statements_count() const { return _statements.size(); }
shared_ptr<ast::blaise_statement_compund>gasp::blaise::ast::make_compound_statement()
{
   return shared_ptr<blaise_statement_compund>(new blaise_statement_compund());
}

//
// ASSIGNMENT STATEMENT
//
blaise_statement_assignment::blaise_statement_assignment(shared_ptr<blaise_generic_memory_location> variable,
                            shared_ptr<blaise_expression> expression) 
                            : blaise_statement(blaise_statement_type::ASSIGNEMENT),
                            _variable(variable),
                            _expression(expression)
                            { }
shared_ptr<blaise_generic_memory_location> blaise_statement_assignment::variable() const { return _variable; }
shared_ptr<blaise_expression> blaise_statement_assignment::expression() const { return _expression; }
shared_ptr<blaise_statement> gasp::blaise::ast::make_assignement_statement(unsigned int line, unsigned int column,
                                                                  shared_ptr<blaise_generic_memory_location> variable,
                                                                  shared_ptr<blaise_expression> expression) {
   if(variable->variable_type() == blaise_variable_type::CONSTANT)
      throw blaise_ast_error(line, column, make_string("Variable '", variable->name() ,"' is constant."));

   if(expression->result_type() != variable->type() && !ast::blaise_ast_utility::can_cast(expression->result_type(), variable->type())){
        throw blaise_ast_error(line, column, make_string("Cannot cast '", expression->result_type(), "' into '",variable->type(),"'."));
     }
   
   return shared_ptr<blaise_statement_assignment>(new blaise_statement_assignment(variable, expression));
}

//
// SUBROUTINE CALL
//
   blaise_statement_subroutine_call::blaise_statement_subroutine_call(std::shared_ptr<blaise_subroutine> subroutine,
            const std::vector<std::shared_ptr<blaise_expression>>& parameters) : 
            blaise_statement(blaise_statement_type::SUBROUTINE_CALL),
            _subroutine(subroutine),
            _actual_parameters(parameters)
            {}
   std::shared_ptr<blaise_subroutine> blaise_statement_subroutine_call::subroutine() const { return _subroutine;}
   unsigned int blaise_statement_subroutine_call::parameters_count() const { return _actual_parameters.size(); }
   std::shared_ptr<blaise_expression> blaise_statement_subroutine_call::actual_parameter(unsigned int index) const { return _actual_parameters.at(index); }

std::shared_ptr<blaise_statement> gasp::blaise::ast::make_blaise_statement_subroutine_call(std::shared_ptr<blaise_subroutine> subroutine,
         const std::vector<std::shared_ptr<blaise_expression>>& parameters) {
            return shared_ptr<blaise_statement_subroutine_call>(new blaise_statement_subroutine_call(subroutine, parameters));
}