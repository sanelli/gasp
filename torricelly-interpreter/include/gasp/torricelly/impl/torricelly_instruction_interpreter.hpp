#pragma once

#include <memory>
#include <functional>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/activation_record.hpp>

namespace gasp::torricelly::interpreter {

class torricelly_interpreter;

class torricelly_instruction_interpreter {
private:
std::shared_ptr<torricelly_interpreter> _interpreter;
torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter);
void execute(const torricelly::torricelly_instruction& instruction, unsigned int &next_instruction, bool& is_jump);
const bool _validate_during_executions = true;

torricelly_activation_record_variable pop_and_validate(std::shared_ptr<torricelly_activation_record> activation_record, torricelly_activation_record_variable_type expected_type);

// General purpose
void execute_noop(const torricelly::torricelly_instruction& instruction);

// Stack management
void execute_dup(const torricelly::torricelly_instruction& instruction);
void inline __execute_pop(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_variable_type extpected_type);
void execute_pop_boolean(const torricelly::torricelly_instruction& instruction);
void execute_pop_integer(const torricelly::torricelly_instruction& instruction);
void execute_pop_float(const torricelly::torricelly_instruction& instruction);
void execute_pop_double(const torricelly::torricelly_instruction& instruction);
void execute_pop_char(const torricelly::torricelly_instruction& instruction);

// Memory management
void inline __execute_load(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_variable_type extpected_type);
void execute_load_boolean(const torricelly::torricelly_instruction& instruction);
void execute_load_integer(const torricelly::torricelly_instruction& instruction);
void execute_load_float(const torricelly::torricelly_instruction& instruction);
void execute_load_double(const torricelly::torricelly_instruction& instruction);
void execute_load_char(const torricelly::torricelly_instruction& instruction);

void inline __execute_store(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_variable_type extpected_type);
void execute_store_boolean(const torricelly::torricelly_instruction& instruction);
void execute_store_integer(const torricelly::torricelly_instruction& instruction);
void execute_store_float(const torricelly::torricelly_instruction& instruction);
void execute_store_double(const torricelly::torricelly_instruction& instruction);
void execute_store_char(const torricelly::torricelly_instruction& instruction);

// Boolean
void inline __execute_binary_boolean(const torricelly::torricelly_instruction &instruction, std::function<bool(bool,bool)> op);
void execute_and(const torricelly::torricelly_instruction& instruction);
void execute_or(const torricelly::torricelly_instruction& instruction);
void execute_not(const torricelly::torricelly_instruction& instruction);

// Mathematics
void inline __execute_binary_integer(const torricelly::torricelly_instruction &instruction, std::function<int(int,int)> op);
void execute_add_integer(const torricelly::torricelly_instruction& instruction);
void execute_sub_integer(const torricelly::torricelly_instruction& instruction);
void execute_mul_integer(const torricelly::torricelly_instruction& instruction);
void execute_div_integer(const torricelly::torricelly_instruction& instruction);
void execute_rem_integer(const torricelly::torricelly_instruction& instruction);
void execute_negate_integer(const torricelly::torricelly_instruction& instruction);

void inline __execute_binary_float(const torricelly::torricelly_instruction &instruction, std::function<float(float,float)> op);
void execute_add_float(const torricelly::torricelly_instruction& instruction);
void execute_sub_float(const torricelly::torricelly_instruction& instruction);
void execute_mul_float(const torricelly::torricelly_instruction& instruction);
void execute_div_float(const torricelly::torricelly_instruction& instruction);
void execute_negate_float(const torricelly::torricelly_instruction& instruction);

void inline __execute_binary_double(const torricelly::torricelly_instruction &instruction, std::function<double(double,double)> op);
void execute_add_double(const torricelly::torricelly_instruction& instruction);
void execute_sub_double(const torricelly::torricelly_instruction& instruction);
void execute_mul_double(const torricelly::torricelly_instruction& instruction);
void execute_div_double(const torricelly::torricelly_instruction& instruction);
void execute_negate_double(const torricelly::torricelly_instruction& instruction);

friend gasp::torricelly::interpreter::torricelly_interpreter;
friend sanelli::memory;

};

}