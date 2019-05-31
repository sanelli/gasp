#include <string>
#include <memory>
#include <algorithm>
#include <bitset>
#include <type_traits>
#include <sstream>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::language::blaise_subroutine::blaise_subroutine(weak_ptr<blaise_module> module, const string &name)
    : _module(module), _name(name), _return_type(blaise_language_type::VOID) {}

std::string gasp::blaise::language::blaise_subroutine::name() const { return _name; }
blaise_language_type gasp::blaise::language::blaise_subroutine::return_type() const { return _return_type; }
void gasp::blaise::language::blaise_subroutine::return_type(const token<blaise_token_type> &type) { _return_type = get_type_from_token(type); }
std::weak_ptr<blaise_module> gasp::blaise::language::blaise_subroutine::module() const { return _module; }

bool gasp::blaise::language::blaise_subroutine::signature_match(const std::string name, const std::vector<language::blaise_language_type>& param_types) const {

   if(_name != name) return false;
   if( _parameters.size() != param_types.size()) return false;
   for(int index=0; index < _parameters.size(); ++index)
      if(_parameters.at(index)->type() != param_types.at(index)) return false;
   return true;
}
std::string gasp::blaise::language::blaise_subroutine::signature_as_string() const {
   std::stringstream stream;
   stream << _name << "(";
   for(int index = 0; index < _parameters.size(); ++index){
      stream << _parameters.at(index)->type();
      if(index != (_parameters.size()-1)) stream << ",";
   }
   stream << ")";
   return stream.str();
}

bool gasp::blaise::language::blaise_subroutine::is(blaise_subroutine_flags flag) const {
   return _flags.test(static_cast<typename std::underlying_type<blaise_subroutine_flags>::type>(flag));
}
void gasp::blaise::language::blaise_subroutine::set(blaise_subroutine_flags flag) {
   _flags.set(static_cast<typename std::underlying_type<blaise_subroutine_flags>::type>(flag));
}
void gasp::blaise::language::blaise_subroutine::reset(blaise_subroutine_flags flag) {
   _flags.reset(static_cast<typename std::underlying_type<blaise_subroutine_flags>::type>(flag));
}

std::shared_ptr<blaise_variable> gasp::blaise::language::blaise_subroutine::get_variable(const token<blaise_token_type> &identifier) const
{
   return get_memory_from_vector(_variables, identifier);
}

std::shared_ptr<blaise_constant> gasp::blaise::language::blaise_subroutine::get_constant(const token<blaise_token_type> &identifier) const
{
   return get_memory_from_vector(_constants, identifier);
}

std::shared_ptr<blaise_subroutine_parameter> gasp::blaise::language::blaise_subroutine::get_parameter(const token<blaise_token_type> &identifier) const
{
   return get_memory_from_vector(_parameters, identifier);
}

std::shared_ptr<blaise_generic_memory_location> gasp::blaise::language::blaise_subroutine::get_memory_location(const token<blaise_token_type> &identifier) const
{
   auto variable = get_variable(identifier);
   if(variable != nullptr) return variable;
   auto constant = get_constant(identifier);
   if(constant != nullptr) return constant;
   auto parameter = get_parameter(identifier);
   if(parameter != nullptr) return parameter;
   return nullptr;
}

shared_ptr<blaise_variable> gasp::blaise::language::blaise_subroutine::add_variable(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Variable '", identifier.value(), "' already defined."));
   auto variable = make_shared<blaise_variable>(identifier, type);
   _variables.push_back(variable);
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
   return variable;
}

shared_ptr<blaise_constant> gasp::blaise::language::blaise_subroutine::add_constant(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_constant - Creating constant for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Constant '", identifier.value(), "' already defined."));
   auto constant = make_shared<blaise_constant>(identifier, type);
   _constants.push_back(constant);
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_constant - Creating constant for " << identifier << " with type " << type << std::endl)
   return constant;
}

shared_ptr<blaise_subroutine_parameter> gasp::blaise::language::blaise_subroutine::add_parameter(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_parameter - Creating parameter for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Parameter '", identifier.value(), "' already defined."));
   auto parameter = make_shared<blaise_subroutine_parameter>(identifier, type);
   _parameters.push_back(parameter);
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_parameter - Creating parameter for " << identifier << " with type " << type << std::endl)
   return parameter;
}
