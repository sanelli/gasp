#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>

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
   if(has_translated(torricelly_modules, blaise_module()->name()))
      return;
}

bool blaise_to_torricelly::translator::has_translated(const std::vector<std::shared_ptr<gasp::torricelly::torricelly_module>> &torricelly_modules,
                                                      const std::string &module_name) const
{
   auto mangled_name = get_mangled_module_name(module_name);
   auto found = std::find_if(torricelly_modules.begin(), torricelly_modules.end(),
                [mangled_name](std::shared_ptr<gasp::torricelly::torricelly_module> module) { return module->module_name() == mangled_name; });
   return found != torricelly_modules.end();
}

std::string blaise_to_torricelly::translator::get_mangled_module_name(const std::string &name) const {
   return name;
}
std::string blaise_to_torricelly::translator::get_mangled_subroutine_name(std::shared_ptr<gasp::blaise::ast::blaise_ast_subroutine> subroutine) const {
  
  auto arity = subroutine->get_arity();
  std::string signature;
  for(auto parameter_index = 0; parameter_index < arity; ++parameter_index) {
     auto type = subroutine->get_parameter_type(parameter_index);
     auto mangled_type_name = get_mangled_type_name(type);
     signature += mangled_type_name;
  }
  auto module_mangled_name = get_mangled_module_name(subroutine->module().lock()->name());
  return sanelli::make_string(module_mangled_name,".", subroutine->name(), "_", signature);
}

std::string blaise_to_torricelly::translator::get_mangled_type_name(std::shared_ptr<gasp::blaise::ast::blaise_ast_type> type) const {
   switch(type->type_type()){
      case blaise::ast::blaise_ast_type_type::PLAIN:
         {
            auto plain_type = blaise::ast::blaise_ast_utility::as_plain_type(type);
            switch(plain_type->system_type()) {
               case blaise::ast::blaise_ast_system_type::BOOLEAN: return "b";
               case blaise::ast::blaise_ast_system_type::CHAR: return "c";
               case blaise::ast::blaise_ast_system_type::VOID : return "v";
               case blaise::ast::blaise_ast_system_type::FLOAT: return "f";
               case blaise::ast::blaise_ast_system_type::INTEGER: return "i";
               default:
                  throw gasp::common::gasp_internal_error("Mangling not implemeneted for system type");
            }
         }
         break;
      default:
         throw gasp::common::gasp_internal_error("Mangling not implemeneted for type");
   }
}
