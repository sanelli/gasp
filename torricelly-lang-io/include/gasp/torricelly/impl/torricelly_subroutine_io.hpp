#pragma once

#include <memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly
{

torricelly_text_output &operator<<(torricelly_text_output &os, const std::shared_ptr<const torricelly_subroutine> subroutine);
torricelly_text_output &operator<<(torricelly_text_output &os, torricelly_subroutine_flag flags);

torricelly_binary_output &operator<<(torricelly_binary_output &os, const std::shared_ptr<const torricelly_subroutine> subroutine);
char to_binary(torricelly_subroutine_flag flag);
torricelly_binary_output &operator<<(torricelly_binary_output &os, torricelly_subroutine_flag flags);

torricelly_subroutine_flag torricelly_subroutine_flag_from_binary(char byte);
torricelly_binary_input &operator>>(torricelly_binary_input &is, torricelly_subroutine_flag &flags);

torricelly_binary_input &operator>>(torricelly_binary_input &is, std::shared_ptr<const torricelly_subroutine>& subroutine);

} // namespace gasp::torricelly
