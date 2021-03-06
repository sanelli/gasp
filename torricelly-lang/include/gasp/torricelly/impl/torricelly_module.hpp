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
   std::vector<std::shared_ptr<torricelly::torricelly_type>> _local_types;
   std::vector<torricelly_value> _local_initial_values;
   std::vector<std::shared_ptr<torricelly_module>> _dependencies;

   torricelly_module(const std::string &module_name);
   
public:
   std::string module_name() const;

   void add_dependency(std::shared_ptr<torricelly_module> dependency);
   unsigned int count_dependencies() const;
   bool has_dependency(std::string dependency) const;
   std::shared_ptr<torricelly_module> get_dependency(std::string dependency) const;
   std::shared_ptr<torricelly_module> get_dependency(unsigned int index) const;
   void replace_dependency(unsigned int index, std::shared_ptr<torricelly_module> dependency);

   unsigned int add_subroutine(std::shared_ptr<torricelly_subroutine> subroutine);
   unsigned int get_number_of_subroutines() const;
   std::shared_ptr<torricelly_subroutine> get_subroutine(unsigned int index) const;
   std::shared_ptr<torricelly_subroutine> get_subroutine(const std::string name) const;
   std::shared_ptr<torricelly_subroutine> get_main() const;

   unsigned int add_local(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value);
   std::shared_ptr<torricelly::torricelly_type> get_local_type(unsigned int index) const;
   torricelly_value get_local_initial_value(unsigned int index) const;
   unsigned int count_locals() const;
   unsigned int get_local_with_string_literal(const std::string& string_literal) const;

   void validate() const;

   friend sanelli::memory;
};

std::shared_ptr<torricelly_module> make_torricelly_module(const std::string &module_name);

} // namespace gasp::torricelly