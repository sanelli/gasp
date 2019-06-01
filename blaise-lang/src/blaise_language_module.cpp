#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <sstream>

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
   std::vector<shared_ptr<blaise_subroutine>> matching_subs_with_cast;

   for(auto subroutine : _subroutines){
      if (subroutine->signature_match_exactly(identifier.value(), param_types))
         return subroutine;
      if(subroutine->signature_match_with_cast(identifier.value(), param_types))
         matching_subs_with_cast.push_back(subroutine);
   }
   
   switch(matching_subs_with_cast.size())
   {
      case 0:
         return nullptr;
      case 1:
         return matching_subs_with_cast.at(0);
      default:
         {
            stringstream stream;
            for(int index = 0; index < matching_subs_with_cast.size(); ++index){
               stream << matching_subs_with_cast.at(0)->signature_as_string();
               if(index != index < matching_subs_with_cast.size()-1) stream << "\n ";
            }
            throw blaise_language_error(identifier.line(), identifier.column(), 
               make_string("Multiple functions matching subroutine call ", identifier.value(), "(" , stream.str(), ")")
            );
         }
   }
}

shared_ptr<blaise_subroutine> gasp::blaise::language::blaise_module::expect_exact_subroutine(const token<blaise_token_type> &identifier,
      const std::vector<blaise_language_type>& param_types) const
{
   for(auto subroutine : _subroutines){
      if (subroutine->signature_match_exactly(identifier.value(), param_types))
         return subroutine;
   }

   stringstream stream;
   for(int index = 0; index < param_types.size(); ++index){
      stream << param_types.at(index);
      if(index != index < param_types.size()-1) stream << "\n ";
   }
   throw blaise_language_error(identifier.line(), identifier.column(), 
               make_string("Cannot find subroutine  ", identifier.value(), "(",  stream.str(), ")")
            );
}

unsigned int gasp::blaise::language::blaise_module::count_subroutine(
      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<blaise_language_type>& param_types) const {

   return std::count_if(std::begin(_subroutines), std::end(_subroutines), 
      [identifier, param_types](auto subroutine){ return subroutine->signature_match_exactly(identifier.value(), param_types); });
}