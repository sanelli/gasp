#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output &torricelly::operator<<(torricelly_text_output &os, const std::shared_ptr<const torricelly_module> module)
{

   os << ".module " << module->module_name() << std::endl;
   os << ".version " << os.version_major() << "." << os.version_minor() << "." << os.version_build() << std::endl;
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
   os << 'T' <<'O'<<'R'<<'R'<<'I'<<'C'<<'E'<<'L'<<'L'<<'Y';
   os << os.version_major() << os.version_minor() << os.version_build();
   os << module->module_name();
   os << (int32_t)module->count_locals();
   for (auto variable_index = 1u; variable_index <= module->count_locals(); ++variable_index)
   {
      os << module->get_local_type(variable_index);
      os << module->get_local_initial_value(variable_index);
   }
   os << (int32_t)module->get_number_of_subroutines();
   for (auto subroutine_index = 1u; subroutine_index <= module->get_number_of_subroutines(); ++subroutine_index)
      os << module->get_subroutine(subroutine_index);
   return os;
}
