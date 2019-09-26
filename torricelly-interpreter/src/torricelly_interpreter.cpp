#include <memory>
#include <string>
#include <stack>

#include <sanelli/sanelli.hpp>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>
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
      _get_parameter(get_parameter) {}
std::shared_ptr<gasp::torricelly::torricelly_module> torricelly_interpreter::main_module() const { return _main_module; }

torricelly_interpreter_status torricelly_interpreter::status() const { return _status; }
torricelly_activation_record_variable torricelly_interpreter::return_value() const { return _return_value; }
void torricelly_interpreter::push_activation_record(std::shared_ptr<gasp::torricelly::torricelly_module> module,
                                                    std::shared_ptr<gasp::torricelly::torricelly_subroutine> subroutine)
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] push_activation_record" << std::endl);
   if (_status != torricelly_interpreter_status::RUNNING && _status != torricelly_interpreter_status::INITIALIZED && _status != torricelly_interpreter_status::ZERO)
      throw torricelly_interpreter_error(sanelli::make_string("Cannot create a new activation record when the interpreter is in status '", to_string(_status), "'."));

   auto activation_record = make_torricelly_activation_record(module, subroutine);

   // Add module variables
   for (auto index = 1U; index <= subroutine->count_locals(); ++index)
      activation_record->store(index - 1, torricelly_activation_record_variable::make(subroutine->get_local_initial_value(index)));

   // Add subroutine variables
   auto mapping = _module_variables_mapping[module->module_name()];
   for (auto index = 1U; index <= module->count_locals(); ++index)
      activation_record->register_module_variable(index - 1, &(mapping->operator[](index)));

   // Push parameters on the stack from left to right
   if (subroutine->count_parameters() > 0)
   {
      if (subroutine->is(torricelly_subroutine_flag::MAIN))
      {
         for (signed int param_count = subroutine->count_parameters(); param_count > 0; --param_count)
         {
            auto variable_type = subroutine->get_local_type(param_count);
            auto input_string = _get_parameter(param_count);
            if (input_string.empty())
            {
               auto default_value = torricelly_value::get_default_value(variable_type);
               activation_record->push(torricelly_activation_record_variable::make(default_value));
            }
            else
            {
               auto value = torricelly_value::get_value_from_string(input_string, variable_type);
               activation_record->push(torricelly_activation_record_variable::make(value));
            }
         }
      }
      else
      {
         for (auto param_count = subroutine->count_parameters(); param_count > 0; --param_count)
            _parameters_passing.push(this->activation_record()->pop());
         while (!_parameters_passing.empty())
         {
            activation_record->push(_parameters_passing.top());
            _parameters_passing.pop();
         }
      }
   }

   _activation_records.push_back(activation_record);
   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] push_activation_record" << std::endl);
}

void torricelly_interpreter::pop_activation_record()
{
   SANELLI_DEBUG("torricelly-interpreter", "[ENTER] pop_activation_record" << std::endl);
   _activation_records.pop_back();
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

   for (auto index = 1U; index <= _main_module->count_locals(); ++index)
   {
      auto initial_value = _main_module->get_local_initial_value(index);
      auto variable = torricelly_activation_record_variable::make(initial_value);
      _module_variables_mapping[_main_module->module_name()]->operator[](index) = variable;
   }
   push_activation_record(_main_module, main_subroutine);

   _status = torricelly_interpreter_status::INITIALIZED;

   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] torricelly_interpreter::initialize" << std::endl);
}
void torricelly_interpreter::run()
{
   if (_status == torricelly_interpreter_status::INITIALIZED)
      _status = torricelly_interpreter_status::RUNNING;

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

   auto current_activation_record = activation_record();
   if (current_activation_record->move_next())
   {
      auto instruction = current_activation_record->instruction();

      SANELLI_DEBUG("torricelly-interpreter", "[INSIDE] Executing (ip = " << current_activation_record->ip() << "): " << to_string(instruction) << std::endl);

      auto next_instruction = 0U;
      auto is_jump = false;
      bool continue_inside_subroutine = _instruction_interpreter->execute(instruction, next_instruction, is_jump);

      if (!continue_inside_subroutine)
      {
         // If getting back to entry point return whatever on the stack
         if (_activation_records.size() == 1)
         {
            if (!torricelly_type_utility::is_void(current_activation_record->subroutine()->return_type()))
               // TODO: Check that the type matches with the expected return type
               _return_value = current_activation_record->peek();
            else
               _return_value = torricelly_activation_record_variable::make(0);
         }
         _activation_records.pop_back();
         _status = torricelly_interpreter_status::FINISHED;
      }

      if (is_jump)
         current_activation_record->jump(next_instruction - 1);
   }
   else
   {
      _status = torricelly_interpreter_status::MALFORMED_SUBROUTINE_ENDING;
   }
   SANELLI_DEBUG("torricelly-interpreter", "[EXIT] step" << std::endl);
}

std::shared_ptr<torricelly_activation_record> torricelly_interpreter::activation_record() const
{
   return _activation_records.back();
}

typename std::vector<std::shared_ptr<torricelly_activation_record>>::const_reverse_iterator torricelly_interpreter::begin_stack_trace() const
{
   return this->_activation_records.rbegin();
}
typename std::vector<std::shared_ptr<torricelly_activation_record>>::const_reverse_iterator torricelly_interpreter::end_stack_trace() const
{
   return this->_activation_records.rend();
}

std::shared_ptr<torricelly_interpreter> gasp::torricelly::interpreter::make_torricelly_interpreter(std::shared_ptr<gasp::torricelly::torricelly_module> main_module, std::function<std::string(unsigned int)> get_parameter)
{
   return sanelli::memory::make_shared<torricelly_interpreter>(main_module, get_parameter);
}
