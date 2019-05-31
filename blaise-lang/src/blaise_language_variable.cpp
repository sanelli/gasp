#include <string>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::language::blaise_generic_memory_location::blaise_generic_memory_location(
                      blaise_variable_type variable_type,
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      _variable_type(variable_type), 
                      _name(identifier.value()), 
                      _type(get_type_from_token(type)) {
   if(identifier.type() != blaise_token_type::IDENTIFIER)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("A token of type '", blaise_token_type::IDENTIFIER,"' was expected but '", identifier.type(),"' was found."));
}

std::string blaise_generic_memory_location::name() const { return _name; }
blaise_language_type blaise_generic_memory_location::type() const { return _type; }
blaise_variable_type blaise_generic_memory_location::variable_type() const { return _variable_type; }

gasp::blaise::language::blaise_variable::blaise_variable(
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      blaise_generic_memory_location(blaise_variable_type::VARIABLE, identifier, type) {}

gasp::blaise::language::blaise_subroutine_parameter::blaise_subroutine_parameter(
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      blaise_generic_memory_location(blaise_variable_type::PARAMETER, identifier, type) {}

gasp::blaise::language::blaise_constant::blaise_constant(
                      const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type)
                      :
                      blaise_generic_memory_location(blaise_variable_type::CONSTANT, identifier, type) {}