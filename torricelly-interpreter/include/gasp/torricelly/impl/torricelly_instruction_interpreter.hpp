#pragma once

#include <memory>
#include <functional>
#include <utility>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_activation_record.hpp>

namespace gasp::torricelly::interpreter {

class torricelly_interpreter;

class torricelly_instruction_interpreter {
private:
std::weak_ptr<torricelly_interpreter> _interpreter;
torricelly_instruction_interpreter(std::shared_ptr<torricelly_interpreter> interpreter);
bool execute(const torricelly::torricelly_instruction& instruction, unsigned int &next_instruction, bool& is_jump);
const bool _validate_during_executions = true;

torricelly_activation_record_local pop_and_validate(std::shared_ptr<torricelly_activation_record> activation_record, torricelly_activation_record_local_type expected_type);
unsigned int get_paramter_and_validate(std::shared_ptr<torricelly_activation_record> activation_record, const torricelly::torricelly_instruction& instruction, const torricelly_inst_ref_type ref_type);

// General purpose
void execute_noop(const torricelly::torricelly_instruction& instruction);

// Stack management
void execute_dup(const torricelly::torricelly_instruction& instruction);
void inline __execute_pop(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_local_type extpected_type);
void execute_pop_boolean(const torricelly::torricelly_instruction& instruction);
void execute_pop_integer(const torricelly::torricelly_instruction& instruction);
void execute_pop_float(const torricelly::torricelly_instruction& instruction);
void execute_pop_double(const torricelly::torricelly_instruction& instruction);
void execute_pop_char(const torricelly::torricelly_instruction& instruction);

// Memory management
void inline __execute_load(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_local_type extpected_type);
void execute_load_boolean(const torricelly::torricelly_instruction& instruction);
void execute_load_byte(const torricelly::torricelly_instruction& instruction);
void execute_load_short(const torricelly::torricelly_instruction& instruction);
void execute_load_long(const torricelly::torricelly_instruction& instruction);
void execute_load_integer(const torricelly::torricelly_instruction& instruction);
void execute_load_float(const torricelly::torricelly_instruction& instruction);
void execute_load_double(const torricelly::torricelly_instruction& instruction);
void execute_load_char(const torricelly::torricelly_instruction& instruction);

void inline __execute_store(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_local_type extpected_type);
void execute_store_boolean(const torricelly::torricelly_instruction& instruction);
void execute_store_byte(const torricelly::torricelly_instruction& instruction);
void execute_store_short(const torricelly::torricelly_instruction& instruction);
void execute_store_integer(const torricelly::torricelly_instruction& instruction);
void execute_store_long(const torricelly::torricelly_instruction& instruction);
void execute_store_float(const torricelly::torricelly_instruction& instruction);
void execute_store_double(const torricelly::torricelly_instruction& instruction);
void execute_store_char(const torricelly::torricelly_instruction& instruction);

void execute_load_array(const torricelly::torricelly_instruction& instruction);
void inline __execute_load_array(const torricelly::torricelly_instruction& instruction, 
                                 torricelly::interpreter::torricelly_activation_record_local_array_underlying_type extpected_type,
                                 std::function<torricelly_activation_record_local(torricelly::interpreter::torricelly_activation_record_local_union)> get_value);
void execute_load_array_boolean(const torricelly::torricelly_instruction& instruction);
void execute_load_array_byte(const torricelly::torricelly_instruction& instruction);
void execute_load_array_short(const torricelly::torricelly_instruction& instruction);
void execute_load_array_integer(const torricelly::torricelly_instruction& instruction);
void execute_load_array_long(const torricelly::torricelly_instruction& instruction);
void execute_load_array_float(const torricelly::torricelly_instruction& instruction);
void execute_load_array_double(const torricelly::torricelly_instruction& instruction);
void execute_load_array_char(const torricelly::torricelly_instruction& instruction);

void execute_store_array(const torricelly::torricelly_instruction& instruction);
void inline __execute_store_array(const torricelly::torricelly_instruction& instruction, 
                                 torricelly_activation_record_local_type expected_stack_type,
                                 torricelly::interpreter::torricelly_activation_record_local_array_underlying_type extpected_type,
                                 std::function<torricelly::interpreter::torricelly_activation_record_local_union(torricelly_activation_record_local)> get_value);
void execute_store_array_boolean(const torricelly::torricelly_instruction& instruction);
void execute_store_array_byte(const torricelly::torricelly_instruction& instruction);
void execute_store_array_short(const torricelly::torricelly_instruction& instruction);
void execute_store_array_integer(const torricelly::torricelly_instruction& instruction);
void execute_store_array_long(const torricelly::torricelly_instruction& instruction);
void execute_store_array_float(const torricelly::torricelly_instruction& instruction);
void execute_store_array_double(const torricelly::torricelly_instruction& instruction);
void execute_store_array_char(const torricelly::torricelly_instruction& instruction);

void __execute_allocate_array(const torricelly::torricelly_instruction& instruction, torricelly_activation_record_local_type extpected_type);
void execute_allocate_short_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_byte_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_integer_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_long_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_char_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_boolean_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_float_array(const torricelly::torricelly_instruction& instruction);
void execute_allocate_double_array(const torricelly::torricelly_instruction& instruction);

void execute_free_array(const torricelly::torricelly_instruction& instruction);

// Boolean
void inline __execute_binary_boolean(const torricelly::torricelly_instruction &instruction, std::function<bool(bool,bool)> op);
void execute_and(const torricelly::torricelly_instruction& instruction);
void execute_or(const torricelly::torricelly_instruction& instruction);
void execute_not(const torricelly::torricelly_instruction& instruction);

// Mathematics
void inline __execute_binary_byte(const torricelly::torricelly_instruction &instruction, std::function<int8_t(int8_t,int8_t)> op);
void execute_add_byte(const torricelly::torricelly_instruction& instruction);
void execute_sub_byte(const torricelly::torricelly_instruction& instruction);
void execute_mul_byte(const torricelly::torricelly_instruction& instruction);
void execute_div_byte(const torricelly::torricelly_instruction& instruction);
void execute_rem_byte(const torricelly::torricelly_instruction& instruction);
void execute_negate_byte(const torricelly::torricelly_instruction& instruction);

void inline __execute_binary_short(const torricelly::torricelly_instruction &instruction, std::function<int16_t(int16_t,int16_t)> op);
void execute_add_short(const torricelly::torricelly_instruction& instruction);
void execute_sub_short(const torricelly::torricelly_instruction& instruction);
void execute_mul_short(const torricelly::torricelly_instruction& instruction);
void execute_div_short(const torricelly::torricelly_instruction& instruction);
void execute_rem_short(const torricelly::torricelly_instruction& instruction);
void execute_negate_short(const torricelly::torricelly_instruction& instruction);

void inline __execute_binary_integer(const torricelly::torricelly_instruction &instruction, std::function<int32_t(int32_t,int32_t)> op);
void execute_add_integer(const torricelly::torricelly_instruction& instruction);
void execute_sub_integer(const torricelly::torricelly_instruction& instruction);
void execute_mul_integer(const torricelly::torricelly_instruction& instruction);
void execute_div_integer(const torricelly::torricelly_instruction& instruction);
void execute_rem_integer(const torricelly::torricelly_instruction& instruction);
void execute_negate_integer(const torricelly::torricelly_instruction& instruction);

void inline __execute_binary_long(const torricelly::torricelly_instruction &instruction, std::function<int64_t(int64_t,int64_t)> op);
void execute_add_long(const torricelly::torricelly_instruction& instruction);
void execute_sub_long(const torricelly::torricelly_instruction& instruction);
void execute_mul_long(const torricelly::torricelly_instruction& instruction);
void execute_div_long(const torricelly::torricelly_instruction& instruction);
void execute_rem_long(const torricelly::torricelly_instruction& instruction);
void execute_negate_long(const torricelly::torricelly_instruction& instruction);

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

// Comparison
void execute_cmp_integer(const torricelly::torricelly_instruction& instruction);
void execute_cmp_float(const torricelly::torricelly_instruction& instruction);
void execute_cmp_double(const torricelly::torricelly_instruction& instruction);
void execute_cmp_boolean(const torricelly::torricelly_instruction& instruction);
void execute_cmp_char(const torricelly::torricelly_instruction& instruction);

// Cast
void execute_cast_y2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_y2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_y2l(const torricelly::torricelly_instruction& instruction);
void execute_cast_y2b(const torricelly::torricelly_instruction& instruction);
void execute_cast_y2c(const torricelly::torricelly_instruction& instruction);
void execute_cast_y2f(const torricelly::torricelly_instruction& instruction);
void execute_cast_y2d(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2l(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2b(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2c(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2f(const torricelly::torricelly_instruction& instruction);
void execute_cast_s2d(const torricelly::torricelly_instruction& instruction);
void execute_cast_l2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_l2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_l2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_l2f(const torricelly::torricelly_instruction& instruction);
void execute_cast_l2d(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2l(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2b(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2c(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2f(const torricelly::torricelly_instruction& instruction);
void execute_cast_i2d(const torricelly::torricelly_instruction& instruction);
void execute_cast_b2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_b2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_b2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_c2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_c2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_c2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_f2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_f2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_f2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_f2l(const torricelly::torricelly_instruction& instruction);
void execute_cast_f2d(const torricelly::torricelly_instruction& instruction);
void execute_cast_d2y(const torricelly::torricelly_instruction& instruction);
void execute_cast_d2s(const torricelly::torricelly_instruction& instruction);
void execute_cast_d2i(const torricelly::torricelly_instruction& instruction);
void execute_cast_d2l(const torricelly::torricelly_instruction& instruction);
void execute_cast_d2f(const torricelly::torricelly_instruction& instruction);

// Jumps
bool __execute_jump(const torricelly::torricelly_instruction& instruction, unsigned int& next_address, std::function<bool(int)> condition);
bool execute_jump(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);
bool execute_jump_lt(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);
bool execute_jump_lte(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);
bool execute_jump_eq(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);
bool execute_jump_neq(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);
bool execute_jump_gte(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);
bool execute_jump_gt(const torricelly::torricelly_instruction& instruction, unsigned int& next_address);

// Invoke
std::pair<std::shared_ptr<torricelly::torricelly_module>, std::shared_ptr<torricelly::torricelly_subroutine>> find_subroutine(std::shared_ptr<torricelly::torricelly_module> module, unsigned int parameter_index);
void execute_static_invoke(const torricelly::torricelly_instruction& instruction);
void execute_native_invoke(const torricelly::torricelly_instruction& instruction);

friend gasp::torricelly::interpreter::torricelly_interpreter;
friend sanelli::memory;

};

}