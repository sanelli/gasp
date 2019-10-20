#include <memory>
#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void torricelly_instruction_interpreter::execute_static_invoke(const torricelly::torricelly_instruction &instruction)
{
   auto interpreter = _interpreter.lock();
   auto activation_record = interpreter->activation_record();
   auto parameter_index = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::MODULE);

   // TODO: Add support for multiple modules
   auto module = activation_record->module();
   auto subroutine_name_local = activation_record->get_module_local(parameter_index);
   auto subroutine_name_ptr = std::static_pointer_cast<std::string>(subroutine_name_local->get_pointer());
   auto subroutine_to_call = module->get_subroutine(*subroutine_name_ptr);

   interpreter->push_activation_record(module, subroutine_to_call);
}