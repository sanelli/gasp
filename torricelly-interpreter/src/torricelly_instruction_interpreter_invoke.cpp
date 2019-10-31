#include <memory>
#include <regex>
#include <string>
#include <utility>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

std::pair<std::shared_ptr<torricelly::torricelly_module>, std::shared_ptr<torricelly::torricelly_subroutine>> torricelly_instruction_interpreter::find_subroutine(std::shared_ptr<torricelly::torricelly_module> module, unsigned int parameter_index) {
   auto interpreter = _interpreter.lock();
   auto activation_record = interpreter->activation_record();

   auto subroutine_name_local = activation_record->get_module_local(parameter_index);
   auto subroutine_name_ptr = subroutine_name_local->get_string_pointer();

   std::string subroutine_name = *subroutine_name_ptr;

   std::regex module_name_regex("^[^\\.]+\\.");
   std::smatch m;
   if (!std::regex_search(subroutine_name, m, module_name_regex))
      throw torricelly_error(sanelli::make_string("Subroutine name '", subroutine_name, "' does contain the module name and cannot be executed."));

   auto module_name = m[0].str();
   module_name = module_name.substr(0, module_name.size() - 1); // remove the trailing .

   auto target_module = module;
   if (module->module_name() != module_name)
      target_module = module->get_dependency(module_name);

   if (target_module == nullptr)
      throw torricelly_error(sanelli::make_string("Cannot find dependency '", module_name, "' in module '", module->module_name(), "'."));

   return std::make_pair(target_module, target_module->get_subroutine(subroutine_name));
}

void torricelly_instruction_interpreter::execute_static_invoke(const torricelly::torricelly_instruction &instruction)
{
   auto interpreter = _interpreter.lock();
   auto activation_record = interpreter->activation_record();
   auto parameter_index = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::MODULE);

   auto module = activation_record->module();
   auto result = find_subroutine(module, parameter_index);

   interpreter->push_activation_record(result.first, result.second);
}