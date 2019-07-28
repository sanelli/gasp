#include <gasp/torricelly/torricelly.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, torricelly_inst_code code) {
   return os << to_string(code);
}

std::ostream& torricelly::operator<<(std::ostream& os, torricelly_inst_code code){
   return os << to_string(code);
}
