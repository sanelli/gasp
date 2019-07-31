#pragma once

#include <ostream>
#include <memory>
#include <gasp/torricelly/impl/torricelly_io.hpp>
#include <gasp/torricelly/impl/torricelly_type.hpp>

namespace gasp::torricelly
{

torricelly_text_output& operator<<(torricelly_text_output& os, std::shared_ptr<torricelly_type> type);

}