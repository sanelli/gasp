#pragma once

#include <ostream>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_inst_code code);
torricelly_binary_output& operator<<(torricelly_binary_output& os, torricelly_inst_code code);

}