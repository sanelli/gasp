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

gasp::blaise::language::blaise_subroutine::blaise_subroutine(const std::string& name): _name(name), _return_type(blaise_expression_type::VOID) { }
gasp::blaise::language::blaise_subroutine::blaise_subroutine(const std::string& name, blaise_expression_type return_type): _name(name), _return_type(return_type) { }

std::string gasp::blaise::language::blaise_subroutine::name() const { return _name; }
blaise_expression_type gasp::blaise::language::blaise_subroutine::return_type() const { return _return_type; }

void gasp::blaise::language::blaise_subroutine::add_variable(const token<blaise_token_type>& identifier, 
                     const token<blaise_token_type>& type){
   _variables.push_back(make_shared<blaise_variable>(identifier, type));
                     }
std::shared_ptr<blaise_variable> gasp::blaise::language::blaise_subroutine::get_variable(const token<blaise_token_type>& identifier) const {
   auto it = std::begin(_variables);
   auto end = std::end(_variables);
   while(it != end) {
      auto variable = *it;
      if(variable->name() == identifier.value())
         return variable;
   }
   return nullptr;
}