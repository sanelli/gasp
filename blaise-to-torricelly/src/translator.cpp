#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <cmath>

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
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_module" << std::endl);

   auto mangled_module_name = get_mangled_module_name(blaise_module->name());
   auto torricelly_module = make_torricelly_module(mangled_module_name);
   std::map<std::string, unsigned int> module_variables_mapping;
   for (auto subroutine_index = 0; subroutine_index < blaise_module->count_subroutines(); ++subroutine_index)
   {
      auto subroutine = blaise_module->get_subroutine(subroutine_index);
      auto subroutine_mangled_name = get_mangled_subroutine_name(subroutine);
      auto variable_index = torricelly_module->add_variable(make_torricelly_system_type(torricelly_system_type_type::STRING_LITERAL),
                                                            torricelly_value::make(subroutine_mangled_name));
      module_variables_mapping[subroutine_mangled_name] = variable_index;
      auto torricelly_subroutine = translate_subroutine(torricelly_module, module_variables_mapping, subroutine);
      torricelly_module->add_subroutine(torricelly_subroutine);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_module" << std::endl);

   return torricelly_module;
}

std::shared_ptr<gasp::torricelly::torricelly_subroutine> blaise_to_torricelly::translator::translate_subroutine(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,
                                                                                                                const std::map<std::string, unsigned int> &module_variables_mapping,
                                                                                                                std::shared_ptr<blaise::ast::blaise_ast_subroutine> subroutine) const
{

   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_subroutine '" << subroutine->name() << "'." << std::endl);
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

      auto store_instruction_code = compute_instruction_code(parameter->type(), torricelly_inst_code::STORE_INTEGER,
                                                             torricelly_inst_code::STORE_FLOAT, torricelly_inst_code::STORE_DOUBLE,
                                                             torricelly_inst_code::STORE_CHAR, torricelly_inst_code::STORE_BOOLEAN);
      auto store_instruction = make_torricelly_instruction(store_instruction_code, torricelly_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(store_instruction);
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

   auto statements_count = subroutine->get_statements_count();
   SANELLI_DEBUG("blaise-to-torricelly", "[INSIDE] translate_subroutine :: translating " << statements_count << " statements." << std::endl);
   unsigned int max_stack_size = subroutine->count_parameters();
   for (auto index = 0UL; index < statements_count; ++index)
   {
      auto statement = subroutine->get_statement(index);
      unsigned int instruction_max_stack_size = 0U;
      translate_statement(torricelly_subroutine, module_variables_mapping, variables_mapping, statement, instruction_max_stack_size);
      max_stack_size = std::max(max_stack_size, instruction_max_stack_size);
   }

   // If it is not a procedure (i.e. return type is not void) we need to push on the stack
   // a variable whose name is the plain name of subroutine itself
   if (!subroutine->return_type()->equals(blaise::ast::make_plain_type(blaise::ast::blaise_ast_system_type::VOID)))
   {
      auto return_variable_it = variables_mapping.find(subroutine->name());
      if (return_variable_it == variables_mapping.end())
         throw blaise_to_torricelly_internal_error("Subroutine with a non VOID return type does not contain a variable names as the subroutine");
      auto return_variable_index = return_variable_it->second;

      // LOAD_XXX [subroutine_name]
      auto load_instruction_code = compute_instruction_code(subroutine->return_type(), torricelly_inst_code::LOAD_INTEGER,
                                                            torricelly_inst_code::LOAD_FLOAT, torricelly_inst_code::LOAD_DOUBLE, torricelly_inst_code::LOAD_CHAR,
                                                            torricelly_inst_code::LOAD_BOOLEAN);
      auto load_instruction = make_torricelly_instruction(load_instruction_code, return_variable_index, torricelly_inst_ref_type::SUBROUTINE);
      torricelly_subroutine->append_instruction(load_instruction);

      // Maximum stack size must be at least one for subroutines returning values
      max_stack_size = std::max(1U, max_stack_size);
   }

   // Add the final RET instruction
   auto ret_instruction = make_torricelly_instruction(torricelly_inst_code::RET);
   torricelly_subroutine->append_instruction(ret_instruction);

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_subroutine" << std::endl);

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
         throw blaise_to_torricelly_internal_error("Type conversion not implemented for blaise system type");
      }
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Type conversion not implemented for blaise type");
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
         throw blaise_to_torricelly_internal_error("VOID type does not have an initial value");
      default:
         throw blaise_to_torricelly_internal_error("Type conversion not implemented for blaise system type");
      }
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Type conversion not implemented for blaise type");
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
      case blaise::ast::blaise_ast_system_type::DOUBLE:
         return "d";
      default:
         throw blaise_to_torricelly_internal_error("Mangling not implemeneted for system type");
      }
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Mangling not implemeneted for type");
   }
}
