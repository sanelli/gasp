#pragma once

#include <memory>

#include <gasp/torricelly/impl/torricelly_io.hpp>
#include <gasp/torricelly/impl/torricelly_variable_value.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_value value);

}
