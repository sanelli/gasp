#pragma once

#include <ostream>

#include <gasp/torricelly/impl/torricelly_io.hpp>
#include <gasp/torricelly/impl/torricelly_instruction_code.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_inst_code code);
std::ostream& operator<<(std::ostream& os, torricelly_inst_code code);

}