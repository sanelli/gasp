#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, const std::shared_ptr<const torricelly_module> module) { 

   os << ".module " << module->module_name() << std::endl;
   os << ".version " << os.version_major() << "." << os.version_minor() << "." << os.version_build() << std::endl;
   os << ".variables " << module->get_number_of_variables() << std::endl;
   for(auto variable_index = 1u; variable_index <= module->get_number_of_variables(); ++variable_index) { 
      os << "   .variable "
         << torricelly_inst_ref_type::MODULE << variable_index << " "
         << module->get_variable_type(variable_index) << " "
         << module->get_initial_value(variable_index) 
         << std::endl
         ;
   }
   for(auto subroutine_index = 1u; subroutine_index <= module->get_number_of_subroutines(); ++subroutine_index)
      os << module->get_subroutine(subroutine_index) << std::endl;
   return os;
}
