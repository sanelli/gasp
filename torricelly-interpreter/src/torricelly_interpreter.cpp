#include <memory>
#include <string>
#include <stack>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/interpreter.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::torricelly::interpreter;

std::string interpreter::to_string(const torricelly_interpreter_status status)
{
   switch (status)
   {
   case torricelly_interpreter_status::ZERO:
      return "zero";
   case torricelly_interpreter_status::INITIALIZED:
      return "initialized";
   case torricelly_interpreter_status::RUNNING:
      return "running";
   case torricelly_interpreter_status::HALTED:
      return "halted";
   case torricelly_interpreter_status::FINISHED:
      return "finished";
   default:
      throw torricelly_interpreter_error("Unknown or unexpected status. It cannot be converted into string");
   }
}

torricelly_interpreter::torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module)
    : _status(torricelly_interpreter_status::ZERO), _main_module(main_module) {}
std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_interpreter::main_module() const { return _main_module; }

torricelly_interpreter_status torricelly_interpreter::status() const { return _status; }

void torricelly_interpreter::initialize() {}
void torricelly_interpreter::run() {}
void torricelly_interpreter::step() {}

torricelly_activation_record_variable torricelly_interpreter::peek_stack() const
{
   if (_status != torricelly_interpreter_status::FINISHED && _status != torricelly_interpreter_status::HALTED)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get top of the stack in status ", to_string(_status)));
   return _activation_records.top()->peek();
}
