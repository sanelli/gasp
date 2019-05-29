#include <string>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::language::blaise_variable::blaise_variable(const token<blaise_token_type>& identifier, 
                      const token<blaise_token_type>& type) : _name(identifier.value()), _type(get_type_from_token(type)) {
   if(identifier.type() != blaise_token_type::IDENTIFIER)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("A token of type '", blaise_token_type::IDENTIFIER,"' was expected but '", identifier.type(),"' was found."));
}

std::string blaise_variable::name() const { return _name; }
blaise_expression_type blaise_variable::type() const { return _type; }
