#include <memory>
#include <functional>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

void inline torricelly_instruction_interpreter::__execute_load(const torricelly::torricelly_instruction &instruction, torricelly_activation_record_local_type extpected_type)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto value = activation_record->load(parameter);

   if (_validate_during_executions)
   {
      if (value.type() != extpected_type)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot load a '", to_string(extpected_type), "'. local is '", to_string(value.type()), "'."));
   }

   activation_record->push(value);
}

void inline torricelly_instruction_interpreter::__execute_store(const torricelly::torricelly_instruction &instruction, torricelly_activation_record_local_type extpected_type)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto value = pop_and_validate(activation_record, extpected_type);
   activation_record->store(parameter, value);
}

void inline torricelly_instruction_interpreter::__execute_load_array(const torricelly::torricelly_instruction &instruction,
                                                                     torricelly::interpreter::torricelly_activation_record_local_array_underlying_type extpected_type,
                                                                     std::function<torricelly_activation_record_local(torricelly::interpreter::torricelly_activation_record_local_union)> get_value)
{
   // Get the idex of the array local
   auto activation_record = _interpreter.lock()->activation_record();
   auto array_index = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto array_local = activation_record->load(array_index);

   // Load the indexes
   auto dimensions = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   std::vector<unsigned int> indexes;
   indexes.resize(dimensions.get_integer());
   for (auto index = dimensions.get_integer() - 1; index >= 0; --index)
   {   
      indexes.at(index) = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER).get_integer();
   }

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

void inline torricelly_instruction_interpreter::__execute_store_array(const torricelly::torricelly_instruction &instruction,
                                                                      torricelly_activation_record_local_type expected_stack_type,
                                                                      torricelly::interpreter::torricelly_activation_record_local_array_underlying_type extpected_type,
                                                                      std::function<torricelly::interpreter::torricelly_activation_record_local_union(torricelly_activation_record_local)> get_value)
{
   // Get the idex of the array local
   auto activation_record = _interpreter.lock()->activation_record();
   auto array_index = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto array_local = activation_record->load(array_index);

   // Load the indexes
   auto dimensions = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER);
   std::vector<unsigned int> indexes;
   indexes.resize(dimensions.get_integer());

   for (auto index = dimensions.get_integer() - 1; index >= 0; --index)
   {
      indexes.at(index) = pop_and_validate(activation_record, torricelly_activation_record_local_type::INTEGER).get_integer();
   }

   // Get the value from the array
   auto array_pointer = array_local.get_array_pointer();
   if (_validate_during_executions && array_pointer->underlying_type() != extpected_type)
   {
      throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                   sanelli::make_string("Cannot load a '", to_string(extpected_type), "'. Arrayariable is '", to_string(array_pointer->underlying_type()), "'."));
   }
   auto computed_index = array_pointer->index(indexes);

   // Pop the value from the stack
   auto value = pop_and_validate(activation_record, expected_stack_type);

   // Set the value converti it into a proper union value
   array_pointer->set(computed_index, get_value(value));
}
void torricelly_instruction_interpreter::execute_load_array(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto value = activation_record->load(parameter);

   if (_validate_during_executions)
   {
      if (value.type() != gasp::torricelly::interpreter::torricelly_activation_record_local_type::POINTER)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot load a '", to_string(gasp::torricelly::interpreter::torricelly_activation_record_local_type::POINTER), "'. Local is '", to_string(value.type()), "'."));
      if (value.get_pointer_underlying_type() != gasp::torricelly::interpreter::torricelly_activation_record_local_underlying_type::ARRAY)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot load a '", to_string(gasp::torricelly::interpreter::torricelly_activation_record_local_type::POINTER), "<",
                                                                           to_string(gasp::torricelly::interpreter::torricelly_activation_record_local_underlying_type::ARRAY), ">'. ",
                                                                           "Local is '", to_string(value.type()), "<", to_string(value.get_pointer_underlying_type()), ">'."));
   }

   activation_record->push(value);
}

void torricelly_instruction_interpreter::execute_store_array(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);
   auto value = pop_and_validate(activation_record, gasp::torricelly::interpreter::torricelly_activation_record_local_type::POINTER);

   if (_validate_during_executions)
   {
      if (value.get_pointer_underlying_type() != gasp::torricelly::interpreter::torricelly_activation_record_local_underlying_type::ARRAY)
         throw torricelly_interpreter_execution_error(activation_record->subroutine()->name(), activation_record->ip(),
                                                      sanelli::make_string("Cannot store a '", to_string(gasp::torricelly::interpreter::torricelly_activation_record_local_type::POINTER), "<",
                                                                           to_string(gasp::torricelly::interpreter::torricelly_activation_record_local_underlying_type::ARRAY), ">'. ",
                                                                           "Local is '", to_string(value.type()), "<", to_string(value.get_pointer_underlying_type()), ">'."));
   }

   activation_record->store(parameter, value);
}

void torricelly_instruction_interpreter::execute_load_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::BOOLEAN);
}

void torricelly_instruction_interpreter::execute_load_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::BYTE);
}
void torricelly_instruction_interpreter::execute_load_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::SHORT);
}
void torricelly_instruction_interpreter::execute_load_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::INTEGER);
}
void torricelly_instruction_interpreter::execute_load_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::LONG);
}

void torricelly_instruction_interpreter::execute_load_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::FLOAT);
}

void torricelly_instruction_interpreter::execute_load_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::DOUBLE);
}

void torricelly_instruction_interpreter::execute_load_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_load(instruction, torricelly_activation_record_local_type::CHAR);
}

void torricelly_instruction_interpreter::execute_store_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::BOOLEAN);
}

void torricelly_instruction_interpreter::execute_store_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::BYTE);
}
void torricelly_instruction_interpreter::execute_store_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::SHORT);
}
void torricelly_instruction_interpreter::execute_store_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::INTEGER);
}
void torricelly_instruction_interpreter::execute_store_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::LONG);
}

void torricelly_instruction_interpreter::execute_store_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::FLOAT);
}

void torricelly_instruction_interpreter::execute_store_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::DOUBLE);
}

void torricelly_instruction_interpreter::execute_store_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_store(instruction, torricelly_activation_record_local_type::CHAR);
}

void torricelly_instruction_interpreter::execute_load_array_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::BOOLEAN,
                        [](auto u) { return torricelly_activation_record_local::make(u._boolean); });
}
void torricelly_instruction_interpreter::execute_load_array_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::BYTE,
                        [](auto u) { return torricelly_activation_record_local::make(u._byte); });
}
void torricelly_instruction_interpreter::execute_load_array_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::SHORT,
                        [](auto u) { return torricelly_activation_record_local::make(u._short); });
}
void torricelly_instruction_interpreter::execute_load_array_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::INTEGER,
                        [](auto u) { return torricelly_activation_record_local::make(u._integer); });
}
void torricelly_instruction_interpreter::execute_load_array_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::LONG,
                        [](auto u) { return torricelly_activation_record_local::make(u._long); });
}
void torricelly_instruction_interpreter::execute_load_array_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::FLOAT,
                        [](auto u) { return torricelly_activation_record_local::make(u._float); });
}
void torricelly_instruction_interpreter::execute_load_array_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::DOUBLE,
                        [](auto u) { return torricelly_activation_record_local::make(u._double); });
}
void torricelly_instruction_interpreter::execute_load_array_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_load_array(instruction,
                        torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::CHAR,
                        [](auto u) { return torricelly_activation_record_local::make(u._char); });
}

void torricelly_instruction_interpreter::execute_store_array_boolean(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::BOOLEAN,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::BOOLEAN,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._boolean = value.get_boolean();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_byte(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::BYTE,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::BYTE,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._byte = value.get_byte();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_short(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::SHORT,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::SHORT,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._short = value.get_short();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_integer(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::INTEGER,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::INTEGER,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._integer = value.get_integer();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_long(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::LONG,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::LONG,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._long = value.get_long();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_float(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::FLOAT,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::FLOAT,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._float = value.get_float();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_double(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::DOUBLE,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::DOUBLE,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._double = value.get_double();
                            return u;
                         });
}
void torricelly_instruction_interpreter::execute_store_array_char(const torricelly::torricelly_instruction &instruction)
{
   __execute_store_array(instruction,
                         torricelly_activation_record_local_type::CHAR,
                         torricelly::interpreter::torricelly_activation_record_local_array_underlying_type::CHAR,
                         [](auto value) {
                            torricelly::interpreter::torricelly_activation_record_local_union u;
                            u._char = value.get_char();
                            return u;
                         });
}

void torricelly_instruction_interpreter::__execute_allocate_array(const torricelly::torricelly_instruction &instruction, torricelly_activation_record_local_type extpected_type)
{
   auto activation_record = _interpreter.lock()->activation_record();

   // Pop the initial value if present
   auto has_initial_value = pop_and_validate(activation_record, gasp::torricelly::interpreter::torricelly_activation_record_local_type::BOOLEAN);
   if (!has_initial_value.get_boolean())
      throw torricelly::interpreter::torricelly_interpreter_execution_error(activation_record->subroutine()->name(),
                                                                            activation_record->ip(),
                                                                            "Cannot execute allocate instruction if an initial value is not present");

   auto initial_value = pop_and_validate(activation_record, extpected_type);

   // Pop the number of dimensions from the stack
   auto number_of_dimensions = pop_and_validate(activation_record, gasp::torricelly::interpreter::torricelly_activation_record_local_type::INTEGER);

   std::vector<unsigned int> dimensions;
   dimensions.resize(number_of_dimensions.get_integer());
   for (auto dimension_index = 0; dimension_index < number_of_dimensions.get_integer(); ++dimension_index)
   {
      auto dimension = pop_and_validate(activation_record, gasp::torricelly::interpreter::torricelly_activation_record_local_type::INTEGER);
      dimensions.at(dimension_index) = (unsigned int)dimension.get_integer();
   }

   // Get the value and push it on the stack
   auto array_value = torricelly_activation_record_local::make(dimensions, initial_value);
   activation_record->push(array_value);
}

void torricelly_instruction_interpreter::execute_allocate_byte_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::BYTE);
}
void torricelly_instruction_interpreter::execute_allocate_short_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::SHORT);
}
void torricelly_instruction_interpreter::execute_allocate_integer_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::INTEGER);
}
void torricelly_instruction_interpreter::execute_allocate_long_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::LONG);
}
void torricelly_instruction_interpreter::execute_allocate_char_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::CHAR);
}
void torricelly_instruction_interpreter::execute_allocate_boolean_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::BOOLEAN);
}
void torricelly_instruction_interpreter::execute_allocate_float_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::FLOAT);
}
void torricelly_instruction_interpreter::execute_allocate_double_array(const torricelly::torricelly_instruction &instruction)
{
   __execute_allocate_array(instruction, gasp::torricelly::interpreter::torricelly_activation_record_local_type::DOUBLE);
}

void torricelly_instruction_interpreter::execute_free_array(const torricelly::torricelly_instruction &instruction)
{
   auto activation_record = _interpreter.lock()->activation_record();
   auto parameter = get_paramter_and_validate(activation_record, instruction, torricelly_inst_ref_type::SUBROUTINE);

   auto value = activation_record->load(parameter);

   if (value.type() != torricelly_activation_record_local_type::POINTER)
      throw torricelly::interpreter::torricelly_interpreter_execution_error(activation_record->subroutine()->name(),
                                                                            activation_record->ip(),
                                                                            "Cannot free a non array variable.");
   if (value.get_pointer_underlying_type() != torricelly_activation_record_local_underlying_type::ARRAY)
      throw torricelly::interpreter::torricelly_interpreter_execution_error(activation_record->subroutine()->name(),
                                                                            activation_record->ip(),
                                                                            "Cannot free a non array variable.");
   auto pointer = value.get_pointer();
   pointer.reset();
}
