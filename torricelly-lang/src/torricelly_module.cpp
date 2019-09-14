#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace sanelli;
using namespace gasp;
using namespace gasp::torricelly;

torricelly_module::torricelly_module(const std::string &module_name)
    : _module_name(module_name) {}

std::string torricelly_module::module_name() const { return _module_name; }

unsigned int torricelly_module::add_subroutine(std::shared_ptr<torricelly_subroutine> subroutine)
{
   _subroutines.push_back(subroutine);
   return _subroutines.size();
}
unsigned int torricelly_module::get_number_of_subroutines() const
{
   return _subroutines.size();
}
std::shared_ptr<torricelly_subroutine> torricelly_module::get_subroutine(unsigned int index) const
{
   return _subroutines.at(index - 1);
}
std::shared_ptr<torricelly_subroutine> torricelly_module::get_main() const
{
   std::shared_ptr<torricelly_subroutine> result = nullptr;
   for (const auto subroutine : _subroutines)
   {
      if (subroutine->is(torricelly_subroutine_flag::MAIN))
       {
          if(result != nullptr)
            throw torricelly_error(sanelli::make_string("More than one subroutine in module '", _module_name, "' is marked as MAIN."));
          result = subroutine;
       }
   }
   return result;
}

unsigned int torricelly_module::add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value)
{
   _variable_types.push_back(type);
   _variable_initial_values.push_back(initial_value);
   return _variable_types.size();
}
std::shared_ptr<torricelly::torricelly_type> torricelly_module::get_variable_type(unsigned int index) const
{
   return _variable_types.at(index - 1);
}
torricelly_value torricelly_module::get_initial_value(unsigned int index) const
{
   return _variable_initial_values.at(index - 1);
}

unsigned int torricelly_module::get_number_of_variables() const
{
   return _variable_types.size();
}
unsigned int torricelly_module::get_variable_with_string_literal(const std::string &string_literal) const
{
   return static_cast<unsigned int>(sanelli::indexof_if(_variable_initial_values.begin(), _variable_initial_values.end(),
                                                        [string_literal](const torricelly_value &value) { return value.is(torricelly_system_type_type::STRING_LITERAL) &&
                                                                                                                 (string_literal == value.get_string_literal()); }));
}

void torricelly_module::validate() const
{
   // Check module name is present
   if (module_name().length() <= 0)
      throw torricelly_error("Invalid module name");

   // Match variable with types
   auto num_of_vars = get_number_of_variables();
   for (auto variable_index = 1; variable_index < num_of_vars; ++variable_index)
   {
      auto initial_value = get_initial_value(variable_index);
      auto variable_type = get_variable_type(variable_index);
      if (!initial_value.match(variable_type))
         throw torricelly_error(sanelli::make_string("Variable at index ", variable_index + 1, " has type ", variable_type, " but initial value has type <type = ", initial_value.type(), ", system_type = ", initial_value.system_type(), "> in module '", module_name(), "'."));
   }

   // Check subroutines
   auto num_of_subroutines = get_number_of_subroutines();
   for (auto subroutine_index = 1; subroutine_index <= num_of_subroutines; ++subroutine_index)
   {
      auto subroutine = get_subroutine(subroutine_index);
      auto subroutine_name = subroutine->name();
      if (!get_variable_with_string_literal(subroutine_name))
         throw torricelly_error(sanelli::make_string("Cannot find a variable of type '", torricelly_system_type_type::STRING_LITERAL, "' and value '", subroutine_name, "' in module '", module_name(), "'."));
      subroutine->validate(num_of_vars);
   }
}

std::shared_ptr<torricelly_module> torricelly::make_torricelly_module(const std::string &module_name)
{
   return memory::make_shared<torricelly_module>(module_name);
}
