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

   // Add dependencies by creating some fake torricelly modules
   for (auto dependency_index = 0; dependency_index < blaise_module->count_dependencies(); ++dependency_index)
   {
      torricelly_module->add_dependency(
          make_torricelly_module(
              get_mangled_module_name(blaise_module->get_dependency(dependency_index)->name())));
   }

   // Add all subroutines to variables list
   for (auto subroutine_index = 0; subroutine_index < blaise_module->count_subroutines(); ++subroutine_index)
   {
      auto subroutine = blaise_module->get_subroutine(subroutine_index);
      auto subroutine_mangled_name = get_mangled_subroutine_name(subroutine);
      auto variable_index = torricelly_module->add_local(make_torricelly_system_type(torricelly_system_type_type::LITERAL_STRING),
                                                         torricelly_value::make(subroutine_mangled_name));
      module_variables_mapping[subroutine_mangled_name] = variable_index;
   }

   // Translate subroutines
   for (auto subroutine_index = 0; subroutine_index < blaise_module->count_subroutines(); ++subroutine_index)
   {
      auto subroutine = blaise_module->get_subroutine(subroutine_index);
      auto torricelly_subroutine = translate_subroutine(torricelly_module, module_variables_mapping, subroutine);
      torricelly_module->add_subroutine(torricelly_subroutine);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_module" << std::endl);

   return torricelly_module;
}

std::shared_ptr<gasp::torricelly::torricelly_subroutine> blaise_to_torricelly::translator::translate_subroutine(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module,
                                                                                                                std::map<std::string, unsigned int> &module_variables_mapping,
                                                                                                                std::shared_ptr<blaise::ast::blaise_ast_subroutine> subroutine) const
{

   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_subroutine '" << subroutine->name() << "'." << std::endl);
   auto subroutine_mangled_name = get_mangled_subroutine_name(subroutine);
   auto return_type = translate_type(subroutine->return_type());
   auto torricelly_subroutine = make_torricelly_subroutine(subroutine_mangled_name, return_type);

   auto flags = torricelly_subroutine_flag::NOTHING;
   flags = flags | torricelly::torricelly_subroutine_flag::STATIC;
   auto is_native = subroutine->is(blaise::ast::blaise_ast_subroutine_flags::NATIVE);
   auto is_main = subroutine->is(blaise::ast::blaise_ast_subroutine_flags::MAIN);
   if (is_main)
      flags = flags | torricelly::torricelly_subroutine_flag::MAIN;
   if (is_native)
      flags = flags | torricelly::torricelly_subroutine_flag::NATIVE;

   torricelly_subroutine->set_flags(flags);

   std::map<std::string, unsigned int> variables_mapping;

   // Crate locals and add STORE from right to left
   if (subroutine->count_parameters() > 0)
   {
      // Fisrt create the locals in the correct order
      for (signed int index = 0; index < subroutine->count_parameters(); ++index)
      {
         auto parameter = subroutine->get_parameter(index);
         auto torricelly_type = translate_type(parameter->type());
         auto initial_value = get_type_initial_value(parameter->type());
         auto torricelly_index = torricelly_subroutine->add_local(torricelly_type, initial_value, true);
         variables_mapping.insert(std::make_pair(parameter->name(), torricelly_index));
      }

      // Then create the STORES in the reverse order
      if (!is_native)
         for (signed int index = subroutine->count_parameters() - 1; index >= 0; --index)
         {
            auto parameter = subroutine->get_parameter(index);
            auto variable_name = parameter->name();
            auto variable_index_it = variables_mapping.find(variable_name);
            if (variable_index_it == variables_mapping.end())
               throw blaise_to_torricelly_error(parameter->line(), parameter->column(), sanelli::make_string("Error while creating STORE instructions for input parameters. Cannot find varibale '", variable_name, "' in subroutine '", subroutine->name(), "'."));

            auto store_instruction_code =
                parameter->type()->type_type() == gasp::blaise::ast::blaise_ast_type_type::ARRAY
                    ? torricelly_inst_code::STORE_ARRAY
                    : compute_instruction_code(parameter->type(),
                                               torricelly_inst_code::STORE_BYTE, torricelly_inst_code::STORE_SHORT, torricelly_inst_code::STORE_INTEGER, torricelly_inst_code::STORE_LONG,
                                               torricelly_inst_code::STORE_FLOAT, torricelly_inst_code::STORE_DOUBLE,
                                               torricelly_inst_code::STORE_CHAR, torricelly_inst_code::STORE_BOOLEAN);
            auto store_instruction = torricelly_instruction::make(store_instruction_code, variable_index_it->second, torricelly_inst_ref_type::SUBROUTINE);
            torricelly_subroutine->append_instruction(store_instruction);
         }
   }

   if (!is_native)
   {
      for (auto index = 0UL; index < subroutine->count_constants(); ++index)
      {
         auto constant = subroutine->get_constant(index);
         auto torricelly_type = translate_type(constant->type());
         auto initial_value = get_type_initial_value(constant->type());
         auto torricelly_index = torricelly_subroutine->add_local(torricelly_type, initial_value, false);
         variables_mapping.insert(std::make_pair(constant->name(), torricelly_index));
      }

      for (auto index = 0UL; index < subroutine->count_variables(); ++index)
      {
         auto variable = subroutine->get_variable(index);
         auto torricelly_type = translate_type(variable->type());
         auto initial_value = get_type_initial_value(variable->type());
         auto torricelly_index = torricelly_subroutine->add_local(torricelly_type, initial_value, false);
         variables_mapping.insert(std::make_pair(variable->name(), torricelly_index));
      }

      SANELLI_DEBUG("blaise-to-torricelly", "[INSIDE] end transalting variables for '" << subroutine->name() << "'." << std::endl);

      auto statements_count = subroutine->get_statements_count();
      SANELLI_DEBUG("blaise-to-torricelly", "[INSIDE] translate_subroutine :: translating " << statements_count << " statements." << std::endl);
      unsigned int max_stack_size = subroutine->count_parameters();
      for (auto index = 0UL; index < statements_count; ++index)
      {
         auto statement = subroutine->get_statement(index);
         unsigned int instruction_max_stack_size = 0U;
         translate_statement(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, statement, instruction_max_stack_size);
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
         auto load_instruction_code = compute_instruction_code(subroutine->return_type(),
                                                               torricelly_inst_code::LOAD_BYTE, torricelly_inst_code::LOAD_SHORT, torricelly_inst_code::LOAD_INTEGER, torricelly_inst_code::LOAD_LONG,
                                                               torricelly_inst_code::LOAD_FLOAT, torricelly_inst_code::LOAD_DOUBLE, torricelly_inst_code::LOAD_CHAR,
                                                               torricelly_inst_code::LOAD_BOOLEAN);
         auto load_instruction = torricelly_instruction::make(load_instruction_code, return_variable_index, torricelly_inst_ref_type::SUBROUTINE);
         torricelly_subroutine->append_instruction(load_instruction);

         // Maximum stack size must be at least one for subroutines returning values
         max_stack_size = std::max(1U, max_stack_size);
         torricelly_subroutine->max_stack_size(max_stack_size);
      }

      // Add the final RET instruction
      auto ret_instruction = torricelly_instruction::make(torricelly_inst_code::RET);
      torricelly_subroutine->append_instruction(ret_instruction);
   }

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_subroutine" << std::endl);

   return torricelly_subroutine;
}

std::shared_ptr<gasp::torricelly::torricelly_type> blaise_to_torricelly::translator::translate_type(std::shared_ptr<blaise::ast::blaise_ast_type> type) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_type" << std::endl);
   switch (type->type_type())
   {
   case blaise::ast::blaise_ast_type_type::PLAIN:
   {
      SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_type for PLAIN " << std::endl);
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
   case blaise::ast::blaise_ast_type_type::ARRAY:
   {
      SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_type for ARRAY " << std::endl);
      auto array_type = blaise::ast::blaise_ast_utility::as_array_type(type);
      auto underlying_type = translate_type(array_type->underlying_type());
      SANELLI_DEBUG("blaise-to-torricelly", "[INSIDE] Got underlying type " << std::endl);
      std::vector<unsigned int> dimensions;
      if (!array_type->is_unbounded())
         dimensions.push_back(array_type->size());
      else
         dimensions.push_back(torricelly_array_type::undefined_dimension());
      SANELLI_DEBUG("blaise-to-torricelly", "[INSIDE] Created dimensions " << std::endl);
      auto torricelly_type = make_torricelly_array_type(underlying_type, dimensions);
      SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_type for ARRAY " << std::endl);
      return torricelly_type;
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Type conversion not implemented for blaise type");
   }
}

gasp::torricelly::torricelly_value blaise_to_torricelly::translator::get_type_initial_value(std::shared_ptr<blaise::ast::blaise_ast_type> type) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] get_type_initial_value" << std::endl);
   auto torricelly_type = translate_type(type);
   auto default_value = torricelly_value::get_default_value(torricelly_type);
   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] get_type_initial_value" << std::endl);
   return default_value;
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
   case blaise::ast::blaise_ast_type_type::ARRAY:
   {
      auto array_type = blaise::ast::blaise_ast_utility::as_array_type(type);
      std::stringstream result;
      result << "a" << get_mangled_type_name(array_type->underlying_type());
      if (array_type->is_unbounded())
         result << "u";
      else
         result << array_type->size();
      return result.str();
   }
   break;
   default:
      throw blaise_to_torricelly_internal_error("Mangling not implemeneted for type");
   }
}

unsigned int blaise_to_torricelly::translator::add_temporary(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &variables_mapping, gasp::torricelly::torricelly_value initial_value) const
{
   if (!torricelly_type_utility::is_system_type(initial_value.type()))
   {
      auto message = sanelli::make_string("Unsupported type. Cannot create a new temporary of type ", to_string(initial_value.type()), ". Only system types are supported.");
      throw blaise_to_torricelly_internal_error(message);
   }
   auto system_type = torricelly_type_utility::as_system_type(initial_value.type());
   auto variable_index = torricelly_subroutine->add_local(make_torricelly_system_type(system_type->system_type()), initial_value);
   variables_mapping[sanelli::make_string("^temp", variable_index)] = variable_index;
   return variable_index;
}

unsigned int blaise_to_torricelly::translator::add_module_subroutine_reference_local(
    std::shared_ptr<gasp::torricelly::torricelly_module> module,
    std::string subroutine_mangled_name,
    std::map<std::string, unsigned int> &module_variables_mapping) const
{
   auto index = module->add_local(make_torricelly_system_type(torricelly_system_type_type::LITERAL_STRING),
                                  torricelly_value::make(subroutine_mangled_name));
   module_variables_mapping[subroutine_mangled_name] = index;
   return index;
}

void blaise_to_torricelly::translator::translate_condition(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine, std::map<std::string, unsigned int> &module_variables_mapping,
                                                           std::map<std::string, unsigned int> &variables_mapping, std::shared_ptr<gasp::blaise::ast::blaise_ast_expression> expression, unsigned int &max_stack_size) const
{
   auto condition_max_stack_size = 0U;
   translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, expression, condition_max_stack_size);

   max_stack_size = std::max(1U, condition_max_stack_size);

   auto true_value_index = add_temporary(torricelly_module, torricelly_subroutine, variables_mapping, torricelly_value::make(true));

   auto load_true_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_BOOLEAN, true_value_index, torricelly_inst_ref_type::SUBROUTINE);
   torricelly_subroutine->append_instruction(load_true_instruction);

   auto comparison_instruction = torricelly_instruction::make(torricelly_inst_code::CMP_BOOLEAN);
   torricelly_subroutine->append_instruction(comparison_instruction);
}

void blaise_to_torricelly::translator::translate_subroutine_call(std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> torricelly_subroutine,
                                                                 std::map<std::string, unsigned int> &module_variables_mapping,
                                                                 std::map<std::string, unsigned int> &variables_mapping,
                                                                 std::shared_ptr<blaise::ast::blaise_ast_subroutine> subroutine,
                                                                 std::function<std::shared_ptr<gasp::blaise::ast::blaise_ast_expression>(unsigned int)> get_parameter,
                                                                 unsigned int &max_stack_size) const
{
   SANELLI_DEBUG("blaise-to-torricelly", "[ENTER] translate_subroutine_call" << std::endl);

   auto number_of_parameters = subroutine->count_parameters();

   max_stack_size = subroutine->return_type()->equals(blaise::ast::make_plain_type(blaise::ast::blaise_ast_system_type::VOID)) ? 0U : 1U;
   max_stack_size = std::max(max_stack_size, number_of_parameters);

   // Push parameters on the stack form left to right
   for (signed int index = 0; index < number_of_parameters; ++index)
   {
      SANELLI_DEBUG("blaise-to-torricelly", "translate_subroutine_call [BEGIN] translating parameter at " << index << std::endl);

      auto parameter_expression = get_parameter(index);
      auto parameter_max_size = 0U;
      translate_expression(torricelly_module, torricelly_subroutine, module_variables_mapping, variables_mapping, parameter_expression, parameter_max_size);
      max_stack_size = std::max(max_stack_size, parameter_max_size);

      SANELLI_DEBUG("blaise-to-torricelly", "translate_subroutine_call [END] translating parameter at " << index << std::endl);
   }

   // Find the subroutine to call index
   auto subroutine_mangled_name = get_mangled_subroutine_name(subroutine);
   SANELLI_DEBUG("blaise-to-torricelly", "translate_subroutine_call::Getting index for subroutine with mangled name '" << subroutine_mangled_name << "'" << std::endl);
   auto subroutine_name_it = module_variables_mapping.find(subroutine_mangled_name);
   unsigned int subroutine_name_index = 0;
   if (subroutine_name_it == module_variables_mapping.end())
      subroutine_name_index = add_module_subroutine_reference_local(torricelly_module, subroutine_mangled_name, module_variables_mapping);
   else
      subroutine_name_index = subroutine_name_it->second;

   // INVOKE instruction
   auto invoke_instruction_code = !subroutine->is(blaise::ast::blaise_ast_subroutine_flags::NATIVE)
                                      ? torricelly_inst_code::STATIC_INVOKE
                                      : torricelly_inst_code::NATIVE_INVOKE;
   auto invoke_instruction = torricelly_instruction::make(invoke_instruction_code, subroutine_name_index, torricelly_inst_ref_type::MODULE);
   torricelly_subroutine->append_instruction(invoke_instruction);

   SANELLI_DEBUG("blaise-to-torricelly", "[EXIT] translate_subroutine_call" << std::endl);
}