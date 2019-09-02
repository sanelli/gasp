#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>

#include <gasp/blaise/ast.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/blaise-to-torricelly/blaise-to-torricelly.hpp>
#include <gasp/common/internal_error.hpp>

#include <sanelli/sanelli.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::blaise;

blaise_to_torricelly::translator::translator(std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module) : _blaise_module(blaise_module) {}
std::shared_ptr<blaise::ast::blaise_ast_module> blaise_to_torricelly::translator::blaise_module() const { return _blaise_module; }

void blaise_to_torricelly::translator::execute(std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> &torricelly_modules) const
{
   if (has_translated(torricelly_modules, blaise_module()->name()))
      return;

   auto module = translate_module(blaise_module());
   torricelly_modules.push_back(module);
}

std::shared_ptr<gasp::torricelly::torricelly_module> blaise_to_torricelly::translator::translate_module(std::shared_ptr<blaise::ast::blaise_ast_module> blaise_module) const
{
   auto mangled_module_name = get_mangled_module_name(blaise_module->name());
   auto torricelly_module = make_torricelly_module(mangled_module_name);
   for (auto subroutine_index = 0; subroutine_index < blaise_module->count_subroutines(); ++subroutine_index)
   {
      auto subroutine = blaise_module->get_subroutine(subroutine_index);
      auto subroutine_mangled_name = get_mangled_subroutine_name(subroutine);
      torricelly_module->add_variable(make_torricelly_system_type(torricelly_system_type_type::STRING_LITERAL),
                                      torricelly_value::make(subroutine_mangled_name));
      auto torricelly_subroutine = translate_subroutine(torricelly_module, subroutine);
      torricelly_module->add_subroutine(torricelly_subroutine);
   }
   return torricelly_module;
}

std::shared_ptr<gasp::torricelly::torricelly_subroutine> blaise_to_torricelly::translator::translate_subroutine(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<blaise::ast::blaise_ast_subroutine> subroutine) const
{
   auto subroutine_mangled_name = get_mangled_subroutine_name(subroutine);
   auto return_type = translate_type(subroutine->return_type());
   auto torricelly_subroutine = make_torricelly_subroutine(subroutine_mangled_name, return_type);

   std::map<std::string, unsigned int> variables_mapping;

   for (auto index = 0UL; index < subroutine->count_parameters(); ++index)
   {
      auto parameter = subroutine->get_parameter(index);
      auto torricelly_type = translate_type(parameter->type());
      auto initial_value = get_type_initial_value(parameter->type());
      auto torricelly_index = torricelly_subroutine->add_variable(torricelly_type, initial_value, true);
      variables_mapping.insert(std::make_pair(parameter->name(), torricelly_index));
   }

   for (auto index = 0UL; index < subroutine->count_constants(); ++index)
   {
      auto constant = subroutine->get_constant(index);
      auto torricelly_type = translate_type(constant->type());
      auto initial_value = get_type_initial_value(constant->type());
      auto torricelly_index = torricelly_subroutine->add_variable(torricelly_type, initial_value, false);
      variables_mapping.insert(std::make_pair(constant->name(), torricelly_index));
   }

   for (auto index = 0UL; index < subroutine->count_variables(); ++index)
   {
      auto variable = subroutine->get_variable(index);
      auto torricelly_type = translate_type(variable->type());
      auto initial_value = get_type_initial_value(variable->type());
      auto torricelly_index = torricelly_subroutine->add_variable(torricelly_type, initial_value, false);
      variables_mapping.insert(std::make_pair(variable->name(), torricelly_index));
   }

   return torricelly_subroutine;
}

std::shared_ptr<gasp::torricelly::torricelly_type> blaise_to_torricelly::translator::translate_type(std::shared_ptr<blaise::ast::blaise_ast_type> type) const
{
   switch (type->type_type())
   {
   case blaise::ast::blaise_ast_type_type::PLAIN:
   {
      auto plain_type = blaise::ast::blaise_ast_utility::as_plain_type(type);
      switch (plain_type->system_type())
      {
      case blaise::ast::blaise_ast_system_type::BOOLEAN:
         return make_torricelly_system_type(torricelly_system_type_type::BOOLEAN);
      case blaise::ast::blaise_ast_system_type::CHAR:
         return make_torricelly_system_type(torricelly_system_type_type::CHAR);
      case blaise::ast::blaise_ast_system_type::DOUBLE:
         return make_torricelly_system_type(torricelly_system_type_type::DOUBLE);
      case blaise::ast::blaise_ast_system_type::FLOAT:
         return make_torricelly_system_type(torricelly_system_type_type::FLOAT);
      case blaise::ast::blaise_ast_system_type::INTEGER:
         return make_torricelly_system_type(torricelly_system_type_type::INTEGER);
      case blaise::ast::blaise_ast_system_type::VOID:
         return make_torricelly_system_type(torricelly_system_type_type::VOID);
      default:
         throw gasp::common::gasp_internal_error("Type conversion not implemented for blaise system type");
      }
   }
   break;
   default:
      throw gasp::common::gasp_internal_error("Type conversion not implemented for blaise type");
   }
}

gasp::torricelly::torricelly_value blaise_to_torricelly::translator::get_type_initial_value(std::shared_ptr<blaise::ast::blaise_ast_type> type) const
{
   switch (type->type_type())
   {
   case blaise::ast::blaise_ast_type_type::PLAIN:
   {
      auto plain_type = blaise::ast::blaise_ast_utility::as_plain_type(type);
      switch (plain_type->system_type())
      {
      case blaise::ast::blaise_ast_system_type::BOOLEAN:
         return torricelly_value::make(false);
      case blaise::ast::blaise_ast_system_type::CHAR:
         return torricelly_value::make((char)0);
      case blaise::ast::blaise_ast_system_type::DOUBLE:
         return torricelly_value::make(0.00);
      case blaise::ast::blaise_ast_system_type::FLOAT:
         return torricelly_value::make(0.00f);
      case blaise::ast::blaise_ast_system_type::INTEGER:
        return torricelly_value::make(0);
      case blaise::ast::blaise_ast_system_type::VOID:
         throw gasp::common::gasp_internal_error("VOID type does not have an initial value");
      default:
         throw gasp::common::gasp_internal_error("Type conversion not implemented for blaise system type");
      }
   }
   break;
   default:
      throw gasp::common::gasp_internal_error("Type conversion not implemented for blaise type");
   }
}

bool blaise_to_torricelly::translator::has_translated(const std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> &torricelly_modules,
                                                      const std::string &module_name) const
{
   auto mangled_name = get_mangled_module_name(module_name);
   auto found = std::find_if(torricelly_modules.begin(), torricelly_modules.end(),
                             [mangled_name](std::shared_ptr<gasp::torricelly::torricelly_module> module) { return module->module_name() == mangled_name; });
   return found != torricelly_modules.end();
}

std::string blaise_to_torricelly::translator::get_mangled_module_name(const std::string &name) const
{
   return name;
}
std::string blaise_to_torricelly::translator::get_mangled_subroutine_name(std::shared_ptr<gasp::blaise::ast::blaise_ast_subroutine> subroutine) const
{

   auto arity = subroutine->get_arity();
   std::string signature;
   for (auto parameter_index = 0; parameter_index < arity; ++parameter_index)
   {
      auto type = subroutine->get_parameter_type(parameter_index);
      auto mangled_type_name = get_mangled_type_name(type);
      signature += mangled_type_name;
   }
   auto module_mangled_name = get_mangled_module_name(subroutine->module().lock()->name());
   return sanelli::make_string(module_mangled_name, ".", subroutine->name(), "_", signature);
}

std::string blaise_to_torricelly::translator::get_mangled_type_name(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> type) const
{
   switch (type->type_type())
   {
   case blaise::ast::blaise_ast_type_type::PLAIN:
   {
      auto plain_type = blaise::ast::blaise_ast_utility::as_plain_type(type);
      switch (plain_type->system_type())
      {
      case blaise::ast::blaise_ast_system_type::BOOLEAN:
         return "b";
      case blaise::ast::blaise_ast_system_type::CHAR:
         return "c";
      case blaise::ast::blaise_ast_system_type::VOID:
         return "v";
      case blaise::ast::blaise_ast_system_type::FLOAT:
         return "f";
      case blaise::ast::blaise_ast_system_type::INTEGER:
         return "i";
      default:
         throw gasp::common::gasp_internal_error("Mangling not implemeneted for system type");
      }
   }
   break;
   default:
      throw gasp::common::gasp_internal_error("Mangling not implemeneted for type");
   }
}
