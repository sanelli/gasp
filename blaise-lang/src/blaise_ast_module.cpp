#include <string>
#include <memory>
#include <algorithm>
#include <vector>

#include <gasp/blaise/ast.hpp>
#include <gasp/blaise/tokenizer/tokens.hpp>
#include <gasp/blaise/tokenizer/tokenizer.hpp>
#include <sanelli/sanelli.hpp>

using namespace sanelli;
using namespace gasp::blaise::ast;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::ast::blaise_ast_module::blaise_ast_module(const token<blaise_token_type> &reference, const std::string& module_name, blaise_ast_module_type type)
    : blaise_ast(reference, blaise_token_type::IDENTIFIER), _name(module_name), _type(type)
{
}

std::string gasp::blaise::ast::blaise_ast_module::name() const { return _name; }
blaise_ast_module_type gasp::blaise::ast::blaise_ast_module::type() const { return _type; }
void gasp::blaise::ast::blaise_ast_module::self(std::weak_ptr<blaise_ast_module> module) { _self = module; }

shared_ptr<blaise_ast_subroutine> gasp::blaise::ast::blaise_ast_module::add_subroutine(const token<blaise_token_type> &identifier)
{
   if(!_self.lock())
      throw std::runtime_error("Module self point not setup");
   auto subroutine = make_blaise_ast_subroutine(_self, identifier.value());
   _subroutines.push_back(subroutine);
   return subroutine;
}

std::shared_ptr<blaise_ast_subroutine> gasp::blaise::ast::blaise_ast_module::get_subroutine(
      const token<blaise_token_type> &identifier,
      const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const
{
   std::vector<shared_ptr<blaise_ast_subroutine>> matching_subs_with_cast;

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
            throw blaise_ast_error(identifier.line(), identifier.column(), 
               sanelli::make_string("Multiple functions matching subroutine call ", identifier.value(), "(" , stream.str(), ")")
            );
         }
   }
}

std::shared_ptr<blaise_ast_subroutine> gasp::blaise::ast::blaise_ast_module::expect_exact_subroutine(
      const token<blaise_token_type> &identifier,
      const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const
{
   for(auto subroutine : _subroutines){
      if (subroutine->signature_match_exactly(identifier.value(), param_types))
         return subroutine;
   }

   throw blaise_ast_error(identifier.line(), identifier.column(), 
               sanelli::make_string("Cannot find subroutine  ", identifier.value(), "(",  param_types, ")")
            );
}

unsigned int gasp::blaise::ast::blaise_ast_module::count_subroutine(
      const gasp::common::token<gasp::blaise::blaise_token_type>& identifier,
      const std::vector<std::shared_ptr<blaise_ast_type>>& param_types) const {

   return std::count_if(std::begin(_subroutines), std::end(_subroutines), 
      [identifier, param_types](auto subroutine){ return subroutine->signature_match_exactly(identifier.value(), param_types); });
}

std::shared_ptr<blaise_ast_module> gasp::blaise::ast::make_blaise_ast_module(const gasp::common::token<gasp::blaise::blaise_token_type>& reference, const std::string& module_name, blaise_ast_module_type type){
   return memory::make_shared<blaise_ast_module>(reference, module_name, type);
}