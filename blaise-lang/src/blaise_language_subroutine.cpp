#include <string>
#include <memory>
#include <algorithm>

#include <gasp/blaise/language.hpp>
#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>
#include <gasp/common/debug.hpp>

using namespace gasp::blaise::language;
using namespace gasp::blaise;
using namespace gasp::common;
using namespace std;

gasp::blaise::language::blaise_subroutine::blaise_subroutine(weak_ptr<blaise_module> module, const string &name)
    : _module(module), _name(name), _return_type(blaise_expression_type::VOID) {}
gasp::blaise::language::blaise_subroutine::blaise_subroutine(weak_ptr<blaise_module> module, const string &name, blaise_expression_type return_type)
    : _module(module), _name(name), _return_type(return_type) {}

std::string gasp::blaise::language::blaise_subroutine::name() const { return _name; }
blaise_expression_type gasp::blaise::language::blaise_subroutine::return_type() const { return _return_type; }
std::weak_ptr<blaise_module> gasp::blaise::language::blaise_subroutine::module() const { return _module; }

void gasp::blaise::language::blaise_subroutine::add_variable(const token<blaise_token_type> &identifier,
                                                             const token<blaise_token_type> &type)
{
   // TODO: Check no function with the same name has already been added
   // TODO: Check no constant with the same name has already been added

   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
   if (get_variable(identifier) != nullptr)
      throw blaise_language_error(identifier.line(), identifier.column(), make_string("Variable '", identifier.value(), "' already defined."));
   _variables.push_back(make_shared<blaise_variable>(identifier, type));
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::add_variable - Creating variable for " << identifier << " with type " << type << std::endl)
}

std::shared_ptr<blaise_variable> gasp::blaise::language::blaise_subroutine::get_variable(const token<blaise_token_type> &identifier) const
{
   GASP_DEBUG("blaise-lang", "[BEGIN] blaise_subroutine::get_variable<" << identifier << ">." << std::endl)
   auto it = std::begin(_variables);
   auto end = std::end(_variables);
   while (it != end)
   {
      auto variable = *it;
      if (variable->name() == identifier.value())
      {
         GASP_DEBUG("blaise-lang", "[INSIDE] blaise_subroutine::get_variable<" << identifier << "> -> MATCH FOUND" << std::endl)
         return variable;
      }
      ++it;
   }
   GASP_DEBUG("blaise-lang", "[END] blaise_subroutine::get_variable<" << identifier << "> -> NO MATCH" << std::endl)
   return nullptr;
}