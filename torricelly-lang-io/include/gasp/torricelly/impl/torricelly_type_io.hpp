#pragma once

#include <ostream>
#include <memory>

#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/impl/torricelly_io.hpp>

namespace gasp::torricelly
{

torricelly_text_output& operator<<(torricelly_text_output& os, const std::shared_ptr<const torricelly_type> type);

}