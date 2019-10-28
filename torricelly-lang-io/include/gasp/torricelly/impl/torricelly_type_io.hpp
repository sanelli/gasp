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

torricelly_type_type torricelly_type_type_from_binary(char byte);
torricelly_system_type_type torricelly_system_type_type_from_binary(char byte);
torricelly_binary_input &operator>>(torricelly_binary_input &is, torricelly_type_type& type);
torricelly_binary_input &operator>>(torricelly_binary_input &is, torricelly_system_type_type& type);
torricelly_binary_input &operator>>(torricelly_binary_input &is, std::shared_ptr<torricelly_type>& type);

} // namespace gasp::torricelly