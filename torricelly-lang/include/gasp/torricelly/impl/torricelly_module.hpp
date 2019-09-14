#pragma once

#include <string>
#include <memory>
#include <vector>

#include <gasp/torricelly/impl/torricelly_subroutine.hpp>
#include <gasp/torricelly/impl/torricelly_type.hpp>
#include <gasp/torricelly/impl/torricelly_value.hpp>

namespace gasp::torricelly
{

class torricelly_module
{
   std::string _module_name;
   std::vector<std::shared_ptr<torricelly_subroutine>> _subroutines;
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _variable_types;
   std::vector<torricelly_value> _variable_initial_values;

   torricelly_module(const std::string &module_name);
   
public:
   std::string module_name() const;

   unsigned int add_subroutine(std::shared_ptr<torricelly_subroutine> subroutine);
   unsigned int get_number_of_subroutines() const;
   std::shared_ptr<torricelly_subroutine> get_subroutine(unsigned int index) const;
   std::shared_ptr<torricelly_subroutine> get_main() const;

   unsigned int add_variable(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value);
   std::shared_ptr<torricelly::torricelly_type> get_variable_type(unsigned int index) const;
   torricelly_value get_initial_value(unsigned int index) const;
   unsigned int get_number_of_variables() const;
   unsigned int get_variable_with_string_literal(const std::string& string_literal) const;

   void validate() const;

   friend sanelli::memory;
};

std::shared_ptr<torricelly_module> make_torricelly_module(const std::string &module_name);

} // namespace gasp::torricelly