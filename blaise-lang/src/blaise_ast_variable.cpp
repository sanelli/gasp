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
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      _variable_type(variable_type), 
                      _name(identifier.value()), 
                      _type(get_type_from_token(type)) {
   if(identifier.type() != blaise_token_type::IDENTIFIER)
      throw blaise_ast_error(identifier.line(), identifier.column(), make_string("A token of type '", blaise_token_type::IDENTIFIER,"' was expected but '", identifier.type(),"' was found."));
}

std::string blaise_ast_generic_memory_location::name() const { return _name; }
blaise_ast_type blaise_ast_generic_memory_location::type() const { return _type; }
blaise_ast_variable_type blaise_ast_generic_memory_location::variable_type() const { return _variable_type; }

gasp::blaise::ast::blaise_ast_variable::blaise_ast_variable(
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      blaise_ast_generic_memory_location(blaise_ast_variable_type::VARIABLE, identifier, type) {}

gasp::blaise::ast::blaise_ast_subroutine_parameter::blaise_ast_subroutine_parameter(
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      blaise_ast_generic_memory_location(blaise_ast_variable_type::PARAMETER, identifier, type) {}

gasp::blaise::ast::blaise_ast_constant::blaise_ast_constant(
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      blaise_ast_generic_memory_location(blaise_ast_variable_type::CONSTANT, identifier, type) {}