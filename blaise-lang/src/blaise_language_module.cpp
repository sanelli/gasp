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

void gasp::blaise::language::blaise_module::add_subroutine(const token<blaise_token_type> &identifier)
{
   if(!_self.lock())
      throw std::runtime_error("Module self point not setup");
   // TODO: Check no function with the same name has already been added
   _subroutines.push_back(make_shared<blaise_subroutine>(_self, identifier.value()));
}

shared_ptr<blaise_subroutine> gasp::blaise::language::blaise_module::get_subroutine(const token<blaise_token_type> &identifier) const
{
   auto it = std::begin(_subroutines);
   auto end = std::end(_subroutines);
   while (it != end)
   {
      auto subroutine = *it;
      if (subroutine->name() == identifier.value())
         return subroutine;
      ++it;
   }
   return nullptr;
}