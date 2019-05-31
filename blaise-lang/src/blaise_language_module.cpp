#include <string>
#include <memory>
#include <algorithm>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::language::blaise_module::blaise_module(const token<blaise_token_type> &identifier, blaise_module_type type)
    : _name(identifier.value()), _type(type)
{
   if (identifier.type() != blaise_token_type::IDENTIFIER)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("A token of type '", blaise_token_type::IDENTIFIER, "' was expected but '", identifier.type(), "' was found."));
}

std::string gasp::blaise::language::blaise_module::name() const { return _name; }
blaise_module_type gasp::blaise::language::blaise_module::type() const { return _type; }
void gasp::blaise::language::blaise_module::self(std::weak_ptr<blaise_module> module) { _self = module; }

shared_ptr<blaise_subroutine> gasp::blaise::language::blaise_module::add_subroutine(const token<blaise_token_type> &identifier)
{
   if(!_self.lock())
      throw std::runtime_error("Module self point not setup");
   // TODO: Check no function with the same name has already been added
   auto subroutine = make_shared<blaise_subroutine>(_self, identifier.value());
   _subroutines.push_back(subroutine);
   return subroutine;
}

shared_ptr<blaise_subroutine> gasp::blaise::language::blaise_module::get_subroutine(const token<blaise_token_type> &identifier,
      const std::vector<blaise_language_type>& param_types) const
{
   auto it = std::begin(_subroutines);
   auto end = std::end(_subroutines);
   while (it != end)
   {
      auto subroutine = *it;
      if (subroutine->signature_match(identifier.value(), param_types))
         return subroutine;
      ++it;
   }
   return nullptr;
}

unsigned int gasp::blaise::language::blaise_module::count_subroutine(
      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<blaise_language_type>& param_types) const {

   return std::count_if(std::begin(_subroutines), std::end(_subroutines), 
      [identifier, param_types](auto subroutine){ return subroutine->signature_match(identifier.value(), param_types); });
}