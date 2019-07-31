
#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, std::shared_ptr<torricelly_subroutine> subroutine) { 

   os << ".subroutine " << subroutine->name() << std::endl;
   os << "   .flags" << subroutine->get_flags() << std::endl;
   os << "   .returns " << subroutine ->return_type() << std::endl;
   os << "   .variables " << subroutine->get_number_of_variables() << std::endl;
   for(unsigned int variable_index = 1; variable_index <= subroutine->get_number_of_variables(); ++variable_index) { 
      os << "      .variable "
         << "#" << variable_index << " "
         << subroutine->get_variable_type(variable_index) << " "
         << subroutine->get_initial_value(variable_index) 
         << (subroutine->is_parameter(variable_index) ? " [parameter]" : "")
         << std::endl
         ;
   }
   return os;
}

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, torricelly_subroutine_flag flags){
   if((flags | torricelly_subroutine_flag::NOTHING) == torricelly_subroutine_flag::NOTHING) return os;

   std::vector<torricelly_subroutine_flag> vectorized;
   if((flags & torricelly_subroutine_flag::STATIC) == torricelly_subroutine_flag::STATIC) vectorized.push_back(torricelly_subroutine_flag::STATIC);
   if((flags & torricelly_subroutine_flag::VIRTUAL) == torricelly_subroutine_flag::VIRTUAL) vectorized.push_back(torricelly_subroutine_flag::VIRTUAL);
   if((flags & torricelly_subroutine_flag::OVERRIDE) == torricelly_subroutine_flag::OVERRIDE) vectorized.push_back(torricelly_subroutine_flag::OVERRIDE);
   if((flags & torricelly_subroutine_flag::FINAL) == torricelly_subroutine_flag::FINAL) vectorized.push_back(torricelly_subroutine_flag::FINAL);
   if((flags & torricelly_subroutine_flag::PUBLIC) == torricelly_subroutine_flag::PUBLIC) vectorized.push_back(torricelly_subroutine_flag::PUBLIC);
   if((flags & torricelly_subroutine_flag::PRIVATE) == torricelly_subroutine_flag::PRIVATE) vectorized.push_back(torricelly_subroutine_flag::PRIVATE);
   if((flags & torricelly_subroutine_flag::PROTECTED) == torricelly_subroutine_flag::PROTECTED) vectorized.push_back(torricelly_subroutine_flag::PROTECTED);

   for(int index = 0; index < vectorized.size(); ++index)
      os << " [" << to_string(vectorized.at(index)) << "]";

   return os;
}
