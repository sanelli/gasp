#include <ostream>
#include <memory>
#include <gasp/torricelly/torricelly.hpp>
#include <gasp/torricelly/torricelly_io.hpp>

using namespace gasp;
using namespace gasp::torricelly;

torricelly_text_output& torricelly::operator<<(torricelly_text_output& os, std::shared_ptr<torricelly_type> type) { 
   return os << to_string(type);
}
