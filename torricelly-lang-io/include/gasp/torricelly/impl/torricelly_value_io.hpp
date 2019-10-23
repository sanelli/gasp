#pragma once

#include <memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly {

std::string to_string(torricelly_value value);
torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_value value);

}
