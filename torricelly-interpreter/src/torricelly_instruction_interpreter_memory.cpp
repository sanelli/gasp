#include <memory>
#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void inline torricelly_instruction_interpreter::__execute_load(const torricelly::torricelly_instruction &instruction, torricelly_activation_record_variable_type extpected_type)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto value = activation_record->load(parameter);
  
   if (_validate_during_executions)
   {
      if (value.type() != extpected_type)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot load a '", to_string(extpected_type), "'. Variable is '", to_string(value.type()), "'."));
   }

   activation_record->push(value);
}

void inline torricelly_instruction_interpreter::__execute_store(const torricelly::torricelly_instruction &instruction, torricelly_activation_record_variable_type extpected_type)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto value = pop_and_validate(activation_record, extpected_type);
   activation_record->store(parameter, value);
}

void torricelly_instruction_interpreter::execute_load_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_variable_type::BOOLEAN);
}

void torricelly_instruction_interpreter::execute_load_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_variable_type::INTEGER);
}

void torricelly_instruction_interpreter::execute_load_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_variable_type::FLOAT);
}

void torricelly_instruction_interpreter::execute_load_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_variable_type::DOUBLE);
}

void torricelly_instruction_interpreter::execute_load_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_variable_type::CHAR);
}

void torricelly_instruction_interpreter::execute_store_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_variable_type::BOOLEAN);
}

void torricelly_instruction_interpreter::execute_store_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_variable_type::INTEGER);
}

void torricelly_instruction_interpreter::execute_store_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_variable_type::FLOAT);
}

void torricelly_instruction_interpreter::execute_store_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_variable_type::DOUBLE);
}

void torricelly_instruction_interpreter::execute_store_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_variable_type::CHAR);
}
