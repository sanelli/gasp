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

public:
   torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module);
   std::shared_ptr<gasp::torricelly::torricelly_module> main_module() const;

   torricelly_interpreter_status status() const;

   void initialize();
   void run();
   void step();

   torricelly_activation_record_variable peek_stack() const;
};

} // namespace gasp::torricelly::interpreter