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

std::shared_ptr<torricelly_subroutine> torricelly_module::get_subroutine(const std::string name) const
{
   std::shared_ptr<torricelly_subroutine> result = nullptr;
   for (const auto subroutine : _subroutines)
   {
      if (subroutine->name() == name)
      {
         if (result != nullptr)
            throw torricelly_error(sanelli::make_string("More than one subroutine in module '", _module_name, "' has name ", name, "."));

         result = subroutine;
      }
   }
   return result;
}

std::shared_ptr<torricelly_subroutine> torricelly_module::get_main() const
{
   std::shared_ptr<torricelly_subroutine> result = nullptr;
   for (const auto subroutine : _subroutines)
   {
      if (subroutine->is(torricelly_subroutine_flag::MAIN))
      {
         if (result != nullptr)
            throw torricelly_error(sanelli::make_string("More than one subroutine in module '", _module_name, "' is marked as MAIN."));

         result = subroutine;
      }
   }
   return result;
}

unsigned int torricelly_module::add_local(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value)
{
   _local_types.push_back(type);
   _local_initial_values.push_back(initial_value);
   return _local_types.size();
}
std::shared_ptr<torricelly::torricelly_type> torricelly_module::get_local_type(unsigned int index) const
{
   return _local_types.at(index - 1);
}
torricelly_value torricelly_module::get_local_initial_value(unsigned int index) const
{
   return _local_initial_values.at(index - 1);
}

unsigned int torricelly_module::count_locals() const
{
   return _local_types.size();
}
unsigned int torricelly_module::get_local_with_string_literal(const std::string &string_literal) const
{
   return static_cast<unsigned int>(sanelli::indexof_if(_local_initial_values.begin(), _local_initial_values.end(),
                                                        [string_literal](const torricelly_value &value) { return torricelly_type_utility::is_string_literal(value.type()) &&
                                                                                                                 (string_literal == value.get_string_literal()); }));
}

void torricelly_module::validate() const
{
   // Check module name is present
   if (module_name().length() <= 0)
      throw torricelly_error("Invalid module name");

   // Match local with types
   auto num_of_vars = count_locals();
   for (auto local_index = 1; local_index < num_of_vars; ++local_index)
   {
      auto initial_value = get_local_initial_value(local_index);
      auto local_type = get_local_type(local_index);
      if (!initial_value.type()->equals(local_type))
         throw torricelly_error(sanelli::make_string("Local at index ", local_index + 1, " has type ", local_type, " but initial value has type <type = ", initial_value.type(), ", system_type = ", to_string(initial_value.type()), "> in module '", module_name(), "'."));
   }

   // Check subroutines
   auto num_of_subroutines = get_number_of_subroutines();
   for (auto subroutine_index = 1; subroutine_index <= num_of_subroutines; ++subroutine_index)
   {
      auto subroutine = get_subroutine(subroutine_index);
      auto subroutine_name = subroutine->name();
      if (!get_local_with_string_literal(subroutine_name))
         throw torricelly_error(sanelli::make_string("Cannot find a local of type '", torricelly_system_type_type::STRING_LITERAL, "' and value '", subroutine_name, "' in module '", module_name(), "'."));
      subroutine->validate(num_of_vars);
   }
}

std::shared_ptr<torricelly_module> torricelly::make_torricelly_module(const std::string &module_name)
{
   return memory::make_shared<torricelly_module>(module_name);
}
