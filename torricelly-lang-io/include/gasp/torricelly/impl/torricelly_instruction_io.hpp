#pragma once

#include<memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_inst_ref_type inst_ref_type);
torricelly_text_output& operator<<(torricelly_text_output& os, const torricelly_instruction& instruction);

torricelly_binary_output& operator<<(torricelly_binary_output& os, torricelly_inst_ref_type inst_ref_type);
torricelly_binary_output& operator<<(torricelly_binary_output& os, const torricelly_instruction& instruction);

}