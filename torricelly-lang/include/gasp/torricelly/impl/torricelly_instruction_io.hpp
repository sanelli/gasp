#pragma once

#include<memory>

#include <gasp/torricelly/impl/torricelly_io.hpp>
#include <gasp/torricelly/impl/torricelly_instruction.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_inst_ref_type inst_ref_type);
torricelly_text_output& operator<<(torricelly_text_output& os, std::shared_ptr<torricelly_instruction> instruction);

}