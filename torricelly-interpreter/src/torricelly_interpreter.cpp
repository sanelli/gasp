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

torricelly_interpreter::torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module, std::function<std::string(unsigned int)> get_parameter)
    : _status(torricelly_interpreter_status::ZERO), 
      _main_module(main_module), 
      _get_parameter(get_parameter) { }
std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_interpreter::main_module() const { return _main_module; }

torricelly_interpreter_status torricelly_interpreter::status() const { return _status; }

void torricelly_interpreter::push_activation_record(std::shared_ptr<gasp::torricelly::torricelly_module> module,
                                                    std::shared_ptr<gasp::torricelly::torricelly_subroutine> subroutine)
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] push_activation_record" << std::endl);
   if (_status != torricelly_interpreter_status::RUNNING && _status != torricelly_interpreter_status::INITIALIZED && _status != torricelly_interpreter_status::ZERO)
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
          for (auto param_count = subroutine->get_number_of_parameters(); param_count > 0; --param_count) { 
             auto variable_type = subroutine->get_variable_type(param_count);
             auto input_string = _get_parameter(param_count);
             if(input_string.empty())
             {
                auto default_value = torricelly_value::get_default_value(variable_type);
                activation_record->push(torricelly_activation_record_variable::make(default_value));
             } else {
                auto value = torricelly_value::get_value_from_string(input_string, variable_type);
                activation_record->push(torricelly_activation_record_variable::make(value));
             }
          }
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
   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] push_activation_record" << std::endl);
}

void torricelly_interpreter::pop_activation_record()
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] pop_activation_record" << std::endl);
   _activation_records.pop();
   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] pop_activation_record" << std::endl);
}

void torricelly_interpreter::initialize()
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] torricelly_interpreter::initialize" << std::endl);

   _instruction_interpreter = sanelli::memory::make_shared<torricelly_instruction_interpreter>(shared_from_this());

   if (_status != torricelly_interpreter_status::ZERO)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot execute initialize when status is '", to_string(_status), "'."));

   _module_variables_mapping[_main_module->module_name()] = std::make_shared<std::map<unsigned int, torricelly_activation_record_variable>>();
   auto main_subroutine = _main_module->get_main();
   if (main_subroutine == nullptr)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get main subroutine from module '", _main_module->module_name(), "'."));

   for (auto index = 1U; index <= _main_module->get_number_of_variables(); ++index)
   {
      auto initial_value = _main_module->get_initial_value(index);
      auto variable = torricelly_activation_record_variable::make(initial_value);
      _module_variables_mapping[_main_module->module_name()]->operator[](index) = variable;
   }
   push_activation_record(_main_module, main_subroutine);

   _status = torricelly_interpreter_status::INITIALIZED;

   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] torricelly_interpreter::initialize" << std::endl);
}
void torricelly_interpreter::run()
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] run" << std::endl);
   while (_status == torricelly_interpreter_status::RUNNING)
      step();
   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] run" << std::endl);
}
void torricelly_interpreter::step()
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] step" << std::endl);

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
   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] step" << std::endl);
}

torricelly_activation_record_variable torricelly_interpreter::peek_stack() const
{
   if (_status != torricelly_interpreter_status::FINISHED && _status != torricelly_interpreter_status::HALTED)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot get top of the stack in status ", to_string(_status)));
   return _activation_records.top()->peek();
}

std::shared_ptr<torricelly_interpreter> gasp::torricelly::interpreter::make_torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module, std::function<std::string(unsigned int)> get_parameter) {
   return sanelli::memory::make_shared<torricelly_interpreter>(main_module, get_parameter);
}