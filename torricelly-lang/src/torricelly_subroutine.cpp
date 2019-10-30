#include <memory>
#include <string>
#include <vector>

#include <sanelli/sanelli.hpp>
#include <gasp/torricelly/torricelly.hpp>

using namespace sanelli;
using namespace gasp;
using namespace gasp::torricelly;

torricelly_subroutine::torricelly_subroutine(const std::string &name,
                                             std::shared_ptr<torricelly::torricelly_type> return_type)
    : _name(name), _return_type(return_type), 
      _flags(torricelly_subroutine_flag::NOTHING), 
      _labels_counter(0), _max_stack_size(0) {}
std::string torricelly_subroutine::name() const { return _name; };
std::shared_ptr<torricelly::torricelly_type> torricelly_subroutine::return_type() const { return _return_type; }

void torricelly_subroutine::set_flags(torricelly_subroutine_flag flags)
{
   _flags = flags;
}
bool torricelly_subroutine::is(torricelly_subroutine_flag flag) const
{
   return (_flags & flag) == flag;
}
torricelly_subroutine_flag torricelly_subroutine::get_flags() const
{
   return _flags;
}
unsigned int torricelly_subroutine::add_local(std::shared_ptr<torricelly::torricelly_type> type, torricelly_value initial_value, bool is_parameter)
{
   _local_types.push_back(type);
   _local_initial_values.push_back(initial_value);
   if (is_parameter)
      _parameters.insert(_local_types.size());
   return _local_types.size();
}
std::shared_ptr<torricelly::torricelly_type> torricelly_subroutine::get_local_type(unsigned int index) const
{
   if (index > _local_types.size())
      throw torricelly_error(sanelli::make_string("Cannot access local type at index ", index, "."));
   return _local_types.at(index - 1);
}
torricelly_value torricelly_subroutine::get_local_initial_value(unsigned int index) const
{
   if (index > _local_initial_values.size())
      throw torricelly_error(sanelli::make_string("Cannot access local initial value at index ", index, "."));
   return _local_initial_values.at(index - 1);
}
unsigned int torricelly_subroutine::count_locals() const
{
   return _local_types.size();
}
unsigned int torricelly_subroutine::count_parameters() const
{
   return _parameters.size();
}
bool torricelly_subroutine::is_parameter(unsigned int local_index) const
{
   return _parameters.count(local_index) > 0;
}
unsigned int torricelly_subroutine::append_instruction(torricelly::torricelly_instruction instruction)
{
   _instructions.push_back(instruction);
   make_labels_map();
   return _instructions.size();
}
void torricelly_subroutine::insert_instruction(unsigned int index, torricelly::torricelly_instruction instruction)
{
   _instructions.insert(_instructions.begin() + index - 1, instruction);
   make_labels_map();
}
void torricelly_subroutine::replace_instruction(unsigned int index, torricelly::torricelly_instruction instruction)
{
   _instructions.at(index - 1) = instruction;
   make_labels_map();
}
void torricelly_subroutine::remove_instruction(unsigned int index)
{
   _instructions.erase(_instructions.begin() + index - 1);
   make_labels_map();
}
torricelly::torricelly_instruction torricelly_subroutine::get_instruction(unsigned int index) const
{
   return _instructions.at(index - 1);
}
unsigned int torricelly_subroutine::get_number_of_instructions() const
{
   return _instructions.size();
}

unsigned int torricelly_subroutine::next_label()
{
   return ++_labels_counter;
}

void torricelly_subroutine::make_labels_map()
{
   _labels_map.clear();
   for (auto address = 1; address < _instructions.size(); ++address)
   {
      auto instruction = get_instruction(address);
      if (instruction.has_label())
         _labels_map[instruction.label()] = address;
   }
}

unsigned int torricelly_subroutine::get_instruction_address(unsigned int label) const
{
   auto it = _labels_map.find(label);
   if (it == _labels_map.end())
      throw torricelly_error(sanelli::make_string("Cannot get address for subroutine '", name(), "' of label '", label, "'."));
   return it->second;
}

unsigned int torricelly_subroutine::get_number_of_labels() const { return _labels_counter; }

unsigned int torricelly_subroutine::max_stack_size() const { return _max_stack_size; }
void torricelly_subroutine::max_stack_size(unsigned int value) { _max_stack_size = value; };

void torricelly_subroutine::validate(unsigned int number_of_module_fields) const
{
   // Check flags
   if (is(torricelly_subroutine_flag::STATIC) && is(torricelly_subroutine_flag::VIRTUAL))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::STATIC, "' and '", torricelly_subroutine_flag::VIRTUAL, "'"));
   if (is(torricelly_subroutine_flag::STATIC) && is(torricelly_subroutine_flag::OVERRIDE))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::STATIC, "' and '", torricelly_subroutine_flag::OVERRIDE, "'"));
   if (is(torricelly_subroutine_flag::STATIC) && is(torricelly_subroutine_flag::FINAL))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::STATIC, "' and '", torricelly_subroutine_flag::FINAL, "'"));
   if (is(torricelly_subroutine_flag::VIRTUAL) && is(torricelly_subroutine_flag::OVERRIDE))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::VIRTUAL, "' and '", torricelly_subroutine_flag::OVERRIDE, "'"));
   if (is(torricelly_subroutine_flag::VIRTUAL) && is(torricelly_subroutine_flag::FINAL))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::VIRTUAL, "' and '", torricelly_subroutine_flag::FINAL, "'"));
   if (is(torricelly_subroutine_flag::PUBLIC) && is(torricelly_subroutine_flag::PRIVATE))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::PUBLIC, "' and '", torricelly_subroutine_flag::PRIVATE, "'"));
   if (is(torricelly_subroutine_flag::PUBLIC) && is(torricelly_subroutine_flag::PROTECTED))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::PUBLIC, "' and '", torricelly_subroutine_flag::PROTECTED, "'"));
   if (is(torricelly_subroutine_flag::PRIVATE) && is(torricelly_subroutine_flag::PROTECTED))
      throw torricelly_error(sanelli::make_string("Subroutine cannot be marked as both '", torricelly_subroutine_flag::PRIVATE, "' and '", torricelly_subroutine_flag::PROTECTED, "'"));

   // Check locals count
   const unsigned int num_of_par = count_parameters();
   const unsigned int num_of_locals = count_locals();
   const unsigned int num_of_labels = get_number_of_labels();
   if (num_of_par > num_of_locals)
      throw torricelly_error(sanelli::make_string("The number of parameters (", num_of_par, ") is greater than the number of locals (", num_of_locals, ")"));

   // Check all locals matches their initial values
   for (auto local_index = 1; local_index <= num_of_locals; ++local_index)
   {
      auto initial_value = get_local_initial_value(local_index);
      auto local_type = get_local_type(local_index);
      if (!initial_value.type()->equals(local_type))
         throw torricelly_error(sanelli::make_string("Local at index ", local_index, " has type ", local_type, " but initial value has type <type = ", initial_value.type(), ", system_type = ", to_string(initial_value.type()), "> in  subroutine '", name(), "'"));
   }

   // Check instructions
   auto num_of_inst = get_number_of_instructions();
   for (auto instruction_index = 1; instruction_index <= num_of_inst; ++instruction_index)
   {
      auto instruction = get_instruction(instruction_index);
      try
      {
         instruction.validate();
      }
      catch (const torricelly_error &inst_error)
      {
         throw torricelly_error(sanelli::make_string("Instruction at ", instruction_index, " is not valid: ", inst_error.what()));
      }
      if (instruction.has_label() && instruction.label() > num_of_labels)
         throw torricelly_error(sanelli::make_string("Instruction at ", instruction_index, " has an invalid label."));
      if (instruction.has_parameter_reference())
      {
         auto ref = instruction.parameter_reference();
         switch (instruction.ref_type())
         {
         case torricelly_inst_ref_type::MODULE:
            if (ref > number_of_module_fields)
               throw torricelly_error(sanelli::make_string("Instruction at ", instruction_index, " refers to an invalid module local."));
            // TODO: Check if instruction match type of module local
            break;
         case torricelly_inst_ref_type::SUBROUTINE:
            if (ref > num_of_locals)
               throw torricelly_error(sanelli::make_string("Instruction at ", instruction_index, " refers to an invalid subroutine local."));
            // TODO: Check if instruction match type of module local
            break;
         case torricelly_inst_ref_type::LABEL:
            if (ref > num_of_labels)
               throw torricelly_error(sanelli::make_string("Instruction at ", instruction_index, " refers to an invalid label."));
            break;
         default:
            throw torricelly_error(sanelli::make_string("Instruction at ", instruction_index, " refers to an unknown parameter reference type."));
         }
      }
   }
}

std::shared_ptr<torricelly::torricelly_subroutine> torricelly::make_torricelly_subroutine(const std::string &name, std::shared_ptr<torricelly::torricelly_type> return_type)
{
   return memory::make_shared<torricelly_subroutine>(name, return_type);
}

torricelly_subroutine_flag torricelly::operator|(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2)
{
   return static_cast<torricelly_subroutine_flag>(
       static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value1) |
       static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value2));
}
torricelly_subroutine_flag torricelly::operator&(torricelly_subroutine_flag value1, torricelly_subroutine_flag value2)
{
   return static_cast<torricelly_subroutine_flag>(
       static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value1) &
       static_cast<std::underlying_type<torricelly_subroutine_flag>::type>(value2));
}

std::ostream &torricelly::operator<<(std::ostream &os, torricelly_subroutine_flag flag)
{
   return os << to_string(flag);
}

std::string torricelly::to_string(torricelly_subroutine_flag flag)
{
   switch (flag)
   {
   case torricelly_subroutine_flag::NOTHING:
      return "";
   case torricelly_subroutine_flag::MAIN:
      return "main";
   case torricelly_subroutine_flag::STATIC:
      return "static";
   case torricelly_subroutine_flag::VIRTUAL:
      return "virtual";
   case torricelly_subroutine_flag::OVERRIDE:
      return "override";
   case torricelly_subroutine_flag::FINAL:
      return "final";
   case torricelly_subroutine_flag::PUBLIC:
      return "public";
   case torricelly_subroutine_flag::PRIVATE:
      return "private";
   case torricelly_subroutine_flag::PROTECTED:
      return "protected";
   case torricelly_subroutine_flag::NATIVE:
      return "native";
   default:
      throw torricelly_error("Cannot write flag into output stream: unknown flag");
   }
}