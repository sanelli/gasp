#pragma once

#include <memory>

#include <gasp/torricelly/impl/torricelly_io.hpp>
#include <gasp/torricelly/impl/torricelly_subroutine.hpp>

namespace gasp::torricelly {

torricelly_text_output& operator<<(torricelly_text_output& os, std::shared_ptr<torricelly_subroutine> subroutine);
torricelly_text_output& operator<<(torricelly_text_output& os, torricelly_subroutine_flag flags);

}
