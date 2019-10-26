#pragma once

#include <ostream>
#include <memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly
{

torricelly_text_output &operator<<(torricelly_text_output &os, const std::shared_ptr<torricelly_type> type);

char to_binary(torricelly_type_type type);
char to_binary(torricelly_system_type_type type);

torricelly_binary_output &operator<<(torricelly_binary_output &os, torricelly_type_type type);
torricelly_binary_output &operator<<(torricelly_binary_output &os, torricelly_system_type_type type);

torricelly_binary_output &operator<<(torricelly_binary_output &os, const std::shared_ptr<torricelly_type> type);

} // namespace gasp::torricelly