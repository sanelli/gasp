#include <memory>
#include <istream>
#include <vector>
#include <utility>
#include <string>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/bf/bf-to-torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;
using namespace gasp::bf;

std::shared_ptr<torricelly_module> bf_to_torricelly_translator::translate(std::string module_name, std::istream &input)
{
   _labels.clear();

   // Create module and subroutine
   auto module = torricelly::make_torricelly_module(module_name);
   auto main = torricelly::make_torricelly_subroutine("main_", torricelly::make_torricelly_system_type(torricelly_system_type_type::VOID));
   main->set_flags(torricelly_subroutine_flag::MAIN | torricelly_subroutine_flag::STATIC);
   module->add_subroutine(main);

   // Add subroutines used locals
   module->add_local(make_torricelly_system_type(torricelly_system_type_type::STRING_LITERAL),
                     torricelly_value::make(module_name + "." + "main_"));
   auto print_char_local_id = module->add_local(make_torricelly_system_type(torricelly_system_type_type::STRING_LITERAL),
                                                torricelly_value::make(std::string("io.print_c")));
   auto read_char_local_id = module->add_local(make_torricelly_system_type(torricelly_system_type_type::STRING_LITERAL),
                                               torricelly_value::make(std::string("io.read_char_")));

   // Add module dependencies
   auto io_dependency = torricelly::make_torricelly_module("io");
   module->add_dependency(io_dependency);

   // Create a variable to be used as pointer to the array location and the array of integers itself
   // TODO: Instead of integers the array should be made of bytes
   auto index_variable_id = main->add_local(torricelly::make_torricelly_system_type(torricelly_system_type_type::INTEGER), torricelly_value::make(0));
   std::vector<unsigned int> array_dimensions;
   array_dimensions.push_back(torricelly::torricelly_array_type::undefined_dimension());
   auto array_type = torricelly::make_torricelly_array_type(torricelly::make_torricelly_system_type(torricelly_system_type_type::INTEGER), array_dimensions);
   auto array_underlying_type = array_type->underlying_type();
   auto array_initial_value = torricelly_value::get_default_value(array_underlying_type);
   auto array_pointer_id = main->add_local(array_type, torricelly_value::make(array_type, array_initial_value));

   // Create temporaries for setting up the array allocation
   auto array_size_id = main->add_local(torricelly::make_torricelly_system_type(torricelly_system_type_type::INTEGER), torricelly_value::make(30000));
   auto array_initial_value_id = main->add_local(torricelly::make_torricelly_system_type(torricelly_system_type_type::INTEGER), torricelly_value::make(0));
   auto array_has_initial_value_id = main->add_local(torricelly::make_torricelly_system_type(torricelly_system_type_type::BOOLEAN), torricelly_value::make(true));

   // Create temporaries for operations
   auto one_id = main->add_local(torricelly::make_torricelly_system_type(torricelly_system_type_type::INTEGER), torricelly_value::make(1));

   // Initialize the array
   auto load_array_size_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, array_size_id, torricelly_inst_ref_type::SUBROUTINE);
   main->append_instruction(load_array_size_instruction);
   auto load_array_dimensions_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
   main->append_instruction(load_array_dimensions_instruction);
   auto load_array_initial_value_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, array_initial_value_id, torricelly_inst_ref_type::SUBROUTINE);
   main->append_instruction(load_array_initial_value_instruction);
   auto load_array_has_initial_value_instruction = torricelly_instruction::make(torricelly_inst_code::LOAD_BOOLEAN, array_has_initial_value_id, torricelly_inst_ref_type::SUBROUTINE);
   main->append_instruction(load_array_has_initial_value_instruction);
   auto allocate_array_instruction = torricelly_instruction::make(torricelly_inst_code::ALLOCATE_INT_ARRAY, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
   main->append_instruction(allocate_array_instruction);

   while (!input.eof())
   {
      char current;
      input.get(current);

      switch (current)
      {
      case '>':
      {
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto add = torricelly_instruction::make(torricelly_inst_code::ADD_INTEGER);
         main->append_instruction(add);
         auto store_index = torricelly_instruction::make(torricelly_inst_code::STORE_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(store_index);
      }
      break;
      case '<':
      {
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto sub = torricelly_instruction::make(torricelly_inst_code::SUBTRACT_INTEGER);
         main->append_instruction(sub);
         auto store_index = torricelly_instruction::make(torricelly_inst_code::STORE_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(store_index);
      }
      break;
      case '+':
      {
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto load_from_array = torricelly_instruction::make(torricelly_inst_code::LOAD_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_from_array);
         load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto add = torricelly_instruction::make(torricelly_inst_code::ADD_INTEGER);
         main->append_instruction(add);
         load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto store_to_array = torricelly_instruction::make(torricelly_inst_code::STORE_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(store_to_array);
      }
      break;
      case '-':
      {
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto load_from_array = torricelly_instruction::make(torricelly_inst_code::LOAD_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_from_array);
         load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto sub = torricelly_instruction::make(torricelly_inst_code::SUBTRACT_INTEGER);
         main->append_instruction(sub);
         load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto store_to_array = torricelly_instruction::make(torricelly_inst_code::STORE_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(store_to_array);
      }
      break;
      case '.':
      {
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto load_from_array = torricelly_instruction::make(torricelly_inst_code::LOAD_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_from_array);
         auto cast_to_char = torricelly_instruction::make(torricelly_inst_code::CAST_INT_CHAR);
         main->append_instruction(cast_to_char);
         auto invoke_print = torricelly_instruction::make(torricelly_inst_code::NATIVE_INVOKE, print_char_local_id, torricelly_inst_ref_type::MODULE);
         main->append_instruction(invoke_print);
      }
      break;
      case ',':
      {
         auto invoke_read = torricelly_instruction::make(torricelly_inst_code::NATIVE_INVOKE, read_char_local_id, torricelly_inst_ref_type::MODULE);
         main->append_instruction(invoke_read);
         auto cast_from_char = torricelly_instruction::make(torricelly_inst_code::CAST_CHAR_INT);
         main->append_instruction(cast_from_char);
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto store_to_array = torricelly_instruction::make(torricelly_inst_code::STORE_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(store_to_array);
      }
      break;
      case '[':
      {
         auto start_label = main->next_label();
         auto end_label = main->next_label();
         _labels.push_back(std::make_pair(start_label, end_label));
         auto noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
         noop.set_label(start_label);
         main->append_instruction(noop);
         auto load_index = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, index_variable_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_index);
         auto load_one = torricelly_instruction::make(torricelly_inst_code::LOAD_INTEGER, one_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_one);
         auto load_from_array = torricelly_instruction::make(torricelly_inst_code::LOAD_ARRAY_INTEGER, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
         main->append_instruction(load_from_array);
         auto jump = torricelly_instruction::make(torricelly_inst_code::JUMP_EQ_ZERO, end_label, torricelly_inst_ref_type::LABEL);
         main->append_instruction(jump);
      }
      break;
      case ']':
      {
         if (_labels.size() < 1)
            throw bf_to_torricelly_error("Missing opening [ in code");

         auto labels = _labels.back();
         auto jump_back = torricelly_instruction::make(torricelly_inst_code::JUMP, labels.first, torricelly_inst_ref_type::LABEL);
         main->append_instruction(jump_back);
         auto noop = torricelly_instruction::make(torricelly_inst_code::NOOP);
         noop.set_label(labels.second);
         main->append_instruction(noop);
         _labels.pop_back();
      }
      break;
      default:
         // Anything else should be treated as a comment
         break;
      }
   }

   // Free the array
   auto free_array_instruction = torricelly_instruction::make(torricelly_inst_code::FREE_ARRAY, array_pointer_id, torricelly_inst_ref_type::SUBROUTINE);
   main->append_instruction(free_array_instruction);

   // Add final RET instruction
   auto ret = torricelly_instruction::make(torricelly_inst_code::RET);
   main->append_instruction(ret);

   if (_labels.size() > 0)
      throw bf_to_torricelly_error("Missing closing ] in code");

   return module;
}