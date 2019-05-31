#include <string>
#include <memory>
#include <algorithm>
#include <bitset>
#include <type_traits>

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
gasp::blaise::language::blaise_subroutine::blaise_subroutine(weak_ptr<blaise_module> module, const string &name, blaise_language_type return_type)
    : _module(module), _name(name), _return_type(return_type) {}

std::string gasp::blaise::language::blaise_subroutine::name() const { return _name; }
blaise_language_type gasp::blaise::language::blaise_subroutine::return_type() const { return _return_type; }
std::weak_ptr<blaise_module> gasp::blaise::language::blaise_subroutine::module() const { return _module; }

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

void gasp::blaise::language::blaise_subroutine::add_variable(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   // TODO: Check no function with the same name has already been added
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Variable '", identifier.value(), "' already defined."));
   _variables.push_back(make_shared<blaise_variable>(identifier, type));
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
}

void gasp::blaise::language::blaise_subroutine::add_constant(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   // TODO: Check no function with the same name has already been added
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_constant - Creating constant for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Constant '", identifier.value(), "' already defined."));
   _constants.push_back(make_shared<blaise_constant>(identifier, type));
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_constant - Creating constant for " << identifier << " with type " << type << std::endl)
}

void gasp::blaise::language::blaise_subroutine::add_parameter(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   // TODO: Check no function with the same name has already been added
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_parameter - Creating parameter for " << identifier << " with type " << type << std::endl)
   if (get_memory_location(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Parameter '", identifier.value(), "' already defined."));
   _parameters.push_back(make_shared<blaise_subroutine_parameter>(identifier, type));
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_parameter - Creating parameter for " << identifier << " with type " << type << std::endl)
}
