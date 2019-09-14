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

void torricelly_interpreter::push_activation_record(std::shared_ptr<gasp::torricelly::torricelly_module> module,
                                                    std::shared_ptr<gasp::torricelly::torricelly_subroutine> subroutine)
{
   if (_status != torricelly_interpreter_status::RUNNING)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot create a new activation record when the interpreter is in status '", to_string(_status), "'."));

   auto activation_record = make_torricelly_activation_record(subroutine);

   // Add module variables
   for (auto index = 1U; index <= subroutine->get_number_of_variables(); ++index)
      activation_record->store(index - 1, torricelly_activation_record_variable::make(subroutine->get_initial_value(index)));

   // Add subroutine variables
   auto mapping = _module_variables_mapping[module->module_name()];
   for (auto index = 1U; index <= module->get_number_of_variables(); ++index)
      activation_record->register_module_variable(index - 1, &(mapping->operator[](index)));

   // Push parameters on the stack
   if (subroutine->get_number_of_parameters() > 0)
   {
      if (subroutine->is(torricelly_subroutine_flag::MAIN))
      {
         throw torricelly_interpreter_error("Interpreter does not support yet parameters for the main subroutine coming from the command line");
      }
      else
      {
         for (auto param_count = subroutine->get_number_of_parameters(); param_count > 0; --param_count)
            _parameters_passing.push(_activation_records.top()->pop());
         while (!_parameters_passing.empty())
         {
            activation_record->push(_parameters_passing.top());
            _parameters_passing.pop();
         }
      }
   }

   _activation_records.push(activation_record);
}

void torricelly_interpreter::pop_activation_record()
{
   _activation_records.pop();
}

void torricelly_interpreter::initialize()
{
   _module_variables_mapping[_main_module->module_name()] = std::make_shared<std::map<unsigned int, torricelly_activation_record_variable>>();
   auto main_subroutine = _main_module->get_main();

   for (auto index = 1U; index <= _main_module->get_number_of_variables(); ++index)
      _module_variables_mapping[_main_module->module_name()]->operator[](index) = torricelly_activation_record_variable::make(_main_module->get_initial_value(index));

   push_activation_record(_main_module, main_subroutine);

   _status = torricelly_interpreter_status::INITIALIZED;
}
void torricelly_interpreter::run()
{
   while (_status == torricelly_interpreter_status::RUNNING)
      step();
}
void torricelly_interpreter::step()
{
   if (_status == torricelly_interpreter_status::INITIALIZED)
      _status = torricelly_interpreter_status::RUNNING;
   if (_status != torricelly_interpreter_status::RUNNING)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot execute next instruction because status pf the interpreter is '", to_string(_status), "'."));

   auto current_activation_record = _activation_records.top();
   if (current_activation_record->move_next())
   {
      auto instruction = current_activation_record->instruction();
   }
   else
   {
      _status = torricelly_interpreter_status::FINISHED;
   }
}

torricelly_activation_record_variable torricelly_interpreter::peek_stack() const
{
   if (_status != torricelly_interpreter_status::FINISHED && _status != torricelly_interpreter_status::HALTED)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get top of the stack in status ", to_string(_status)));
   return _activation_records.top()->peek();
}
