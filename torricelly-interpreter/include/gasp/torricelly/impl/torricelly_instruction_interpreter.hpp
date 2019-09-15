#pragma once

#include <memory>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

namespace gasp::torricelly::interpreter {

class torricelly_interpreter;

class torricelly_instruction_interpreter {
private:
std::shared_ptr<torricelly_interpreter> _interpreter;
torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter);
void execute(const torricelly::torricelly_instruction& instruction, unsigned int &next_instruction, bool& is_jump);
bool validate_during_executions() const;

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

void inline __execute_load(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_variable_type extpected_type);
void execute_load_boolean(const torricelly::torricelly_instruction& instruction);
void execute_load_integer(const torricelly::torricelly_instruction& instruction);
void execute_load_float(const torricelly::torricelly_instruction& instruction);
void execute_load_double(const torricelly::torricelly_instruction& instruction);
void execute_load_char(const torricelly::torricelly_instruction& instruction);

friend gasp::torricelly::interpreter::torricelly_interpreter;
friend sanelli::memory;

};

}