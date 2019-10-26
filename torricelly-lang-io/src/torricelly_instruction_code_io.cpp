#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, torricelly_inst_code code) {
   return os << to_string(code);
}

torricelly_binary_output& torricelly::operator<<(torricelly_binary_output& os, torricelly_inst_code code) {
   return os << (int16_t) code;
}
