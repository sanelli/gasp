#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, torricelly_inst_ref_type inst_ref_type){
   return os << to_string(inst_ref_type);
}

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, const torricelly_instruction& instruction){
   return os << to_string(instruction);
}