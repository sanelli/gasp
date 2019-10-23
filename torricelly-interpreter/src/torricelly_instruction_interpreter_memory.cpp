#include <memory>
#include <functional>

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

void inline torricelly_instruction_interpreter::__execute_load_array(const torricelly::torricelly_instruction &instruction,
                                                                     torricelly::interpreter::torricelly_activation_record_variable_array_underlying_type extpected_type,
                                                                     std::function<torricelly_activation_record_variable(torricelly::interpreter::torricelly_activation_record_variable_union)> get_value)
{
   // Get the idex of the array local
   auto activation_record = _interpreter.lock()->activation_record();
   auto array_index = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto array_local = activation_record->load(array_index);

   // Load the indexes
   auto dimensions = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER);
   std::vector<unsigned int> indexes;
   indexes.resize(dimensions.get_integer());
   for (auto index = dimensions.get_integer() - 1; index >= 0; ++index)
      indexes.at(index) = pop_and_validate(activation_record, torricelly_activation_record_variable_type::INTEGER).get_integer();

   // Get the value from the array
   auto array_pointer = array_local.get_array_pointer();
   if (_validate_during_executions && array_pointer->underlying_type() != extpected_type)
   {
      throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                   sanelli::make_string("Cannot load a '", to_string(extpected_type), "'. Arrayariable is '", to_string(array_pointer->underlying_type()), "'."));
   }
   auto computed_index = array_pointer->index(indexes);
   auto value_union = array_pointer->get(computed_index);

   // Convert the union value into a value that can be pushed on the stack
   auto value = get_value(value_union);

   // Push on the stack
   activation_record->push(value);
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

void torricelly_instruction_interpreter::execute_load_array_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_variable_array_underlying_type::BOOLEAN,
                        [](auto u) { return torricelly_activation_record_variable::make(u._boolean); });
}
void torricelly_instruction_interpreter::execute_load_array_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_variable_array_underlying_type::INTEGER,
                        [](auto u) { return torricelly_activation_record_variable::make(u._integer); });
}
void torricelly_instruction_interpreter::execute_load_array_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_variable_array_underlying_type::FLOAT,
                        [](auto u) { return torricelly_activation_record_variable::make(u._float); });
}
void torricelly_instruction_interpreter::execute_load_array_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_variable_array_underlying_type::DOUBLE,
                        [](auto u) { return torricelly_activation_record_variable::make(u._double); });
}
void torricelly_instruction_interpreter::execute_load_array_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_variable_array_underlying_type::CHAR,
                        [](auto u) { return torricelly_activation_record_variable::make(u._char); });
}
