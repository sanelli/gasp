#include <string>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::ast;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::ast::blaise_ast_generic_memory_location::blaise_ast_generic_memory_location(
                      blaise_ast_variable_type variable_type,
                      const token<blaise_token_type>& reference,
                      const string& identifier, 
                      blaise_ast_type type)
            : blaise_ast(reference, blaise_token_type::IDENTIFIER),
            _variable_type(variable_type), 
            _name(identifier), 
            _type(type) {
}

string blaise_ast_generic_memory_location::name() const { return _name; }
blaise_ast_type blaise_ast_generic_memory_location::type() const { return _type; }
blaise_ast_variable_type blaise_ast_generic_memory_location::variable_type() const { return _variable_type; }

gasp::blaise::ast::blaise_ast_variable::blaise_ast_variable(
                      const token<blaise_token_type>& reference,
                      const string& identifier, 
                      blaise_ast_type type)
         :
         blaise_ast_generic_memory_location(blaise_ast_variable_type::VARIABLE, 
         reference, identifier, type) {}

gasp::blaise::ast::blaise_ast_subroutine_parameter::blaise_ast_subroutine_parameter(
                      const token<blaise_token_type>& reference,
                      const string& identifier, 
                      blaise_ast_type type)
         :
         blaise_ast_generic_memory_location(blaise_ast_variable_type::PARAMETER, 
         reference, identifier, type) {}

gasp::blaise::ast::blaise_ast_constant::blaise_ast_constant(
                      const token<blaise_token_type>& reference,
                      const string& identifier, 
                      blaise_ast_type type)
         :
         blaise_ast_generic_memory_location(blaise_ast_variable_type::CONSTANT, 
         reference, identifier, type) {}