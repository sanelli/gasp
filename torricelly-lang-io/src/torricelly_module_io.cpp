#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

const std::string torricelly_text_header{".torricelly-text"};
const std::string torricelly_binary_header{".torricelly-binary"};

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const std::shared_ptr<const torricelly_module> module)
{
   os << torricelly_text_header << std::endl;
   os << ".version " << os.version_major() << "." << os.version_minor() << "." << os.version_build() << std::endl;
   os << ".module " << module->module_name() << std::endl;
   os << ".dependencies " << module->count_dependencies() << std::endl;
   for (auto dependency_index = 0; dependency_index < module->count_dependencies(); ++dependency_index)
      os << "   .dependency " << module->get_dependency(dependency_index)->module_name() << std::endl;
   os << ".locals " << module->count_locals() << std::endl;
   for (auto variable_index = 1u; variable_index <= module->count_locals(); ++variable_index)
   {
      os << "   .local "
         << torricelly_inst_ref_type::MODULE << variable_index << " "
         << module->get_local_type(variable_index) << " "
         << module->get_local_initial_value(variable_index)
         << std::endl;
   }
   os << std::endl;
   for (auto subroutine_index = 1u; subroutine_index <= module->get_number_of_subroutines(); ++subroutine_index)
      os << module->get_subroutine(subroutine_index) << std::endl;
   os << ".end module" << std::endl;

   return os;
}

torricelly_binary_output &torricelly::operator<<(torricelly_binary_output &os, const std::shared_ptr<const torricelly_module> module)
{
   for (const char c : torricelly_binary_header)
      os << (unsigned char) c;
   os << os.version_major() << os.version_minor() << os.version_build();
   os << module->module_name();
   os << (int32_t)module->count_dependencies();
   for (auto dependency_index = 0; dependency_index < module->count_dependencies(); ++dependency_index)
      os << module->get_dependency(dependency_index)->module_name();
   os << (int32_t)module->count_locals();
   for (auto local_index = 1u; local_index <= module->count_locals(); ++local_index)
   {
      os << module->get_local_type(local_index);
      os << module->get_local_initial_value(local_index);
   }
   os << (int32_t)module->get_number_of_subroutines();
   for (auto subroutine_index = 1u; subroutine_index <= module->get_number_of_subroutines(); ++subroutine_index)
      os << module->get_subroutine(subroutine_index);
   return os;
}

torricelly_binary_input &torricelly::operator>>(torricelly_binary_input &is, std::shared_ptr<torricelly_module> &module)
{
   int header_size = torricelly_binary_header.size();
   std::string header;
   unsigned char byte;
   for (auto index = 0; index < header_size; ++index)
   {
      is >> byte;
      header += byte;
   }

   if (header != torricelly_binary_header)
      throw torricelly_error("Cannot read stream. Header is not of the expected format.");

   int32_t major, minor, build;
   is >> major;
   is >> minor;
   is >> build;

   if (major != is.version_major() ||
       major != is.version_major() ||
       major != is.version_major())
      throw torricelly_error("Cannot read stream. Unsupported version.");

   std::string module_name;
   is >> module_name;

   auto result = torricelly::make_torricelly_module(module_name);

   int32_t dependencies_count;
   is >> dependencies_count;
   for (auto dependency_index = 0; dependency_index < dependencies_count; ++dependency_index)
   {
      std::string dependency;
      is >> dependency;
      auto dependency_module = torricelly::make_torricelly_module(dependency);
      result->add_dependency(dependency_module);
   }

   int32_t locals_count;
   is >> locals_count;
   for (auto local_index = 0; local_index < locals_count; ++local_index)
   {
      std::shared_ptr<torricelly_type> local_type;
      is >> local_type;
      torricelly_value local_value = torricelly_value_from_binary(is, local_type);
      result->add_local(local_type, local_value);
   }

   int32_t subroutine_count;
   is >> subroutine_count;
   for (auto subroutine_index = 0; subroutine_index < subroutine_count; ++subroutine_index)
   {
      std::shared_ptr<torricelly_subroutine> subroutine;
      is >> subroutine;
      result->add_subroutine(subroutine);
   }

   module = result;

   return is;
}