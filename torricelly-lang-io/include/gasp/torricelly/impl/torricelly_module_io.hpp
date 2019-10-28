#pragma once

#include <memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, const std::shared_ptr<const torricelly_module> module);
torricelly_binary_output& operator<<(torricelly_binary_output& os, const std::shared_ptr<const torricelly_module> module);

torricelly_binary_input& operator>>(torricelly_binary_input& is, std::shared_ptr<torricelly_module>& module);


}