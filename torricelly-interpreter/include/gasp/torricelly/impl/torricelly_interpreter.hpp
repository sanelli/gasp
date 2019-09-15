#pragma once

#include <memory>
#include <stack>
#include <string>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/activation_record.hpp>
#include <gasp/torricelly/impl/activation_record_variable.hpp>

namespace gasp::torricelly::interpreter
{

enum class torricelly_interpreter_status : unsigned char {
   ZERO,
   INITIALIZED,
   RUNNING,
   HALTED,
   FINISHED
};

std::string to_string(const torricelly_interpreter_status status);

class torricelly_interpreter
{
   std::shared_ptr<gasp::torricelly::torricelly_module> _main_module;
   std::stack<std::shared_ptr<torricelly_activation_record>> _activation_records;
   torricelly_interpreter_status _status;
   std::stack<torricelly_activation_record_variable> _parameters_passing;
   std::map<std::string, std::shared_ptr<std::map<unsigned int, torricelly_activation_record_variable>>> _module_variables_mapping;
   std::function<std::string(unsigned int)> _get_parameter;

   void push_activation_record(std::shared_ptr<gasp::torricelly::torricelly_module> module, std::shared_ptr<gasp::torricelly::torricelly_subroutine> subroutine);
   void pop_activation_record();

public:
   torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module, std::function<std::string(unsigned int)> get_parameter);
   std::shared_ptr<gasp::torricelly::torricelly_module> main_module() const;

   torricelly_interpreter_status status() const;

   void initialize();
   void run();
   void step();

   torricelly_activation_record_variable peek_stack() const;
};

} // namespace gasp::torricelly::interpreter