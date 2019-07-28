#pragma once

#include <ostream>
#include <gasp/torricelly/impl/torricelly_type.hpp>

namespace gasp::torricelly
{

std::ostream &operator<<(std::ostream &os, torricelly_type_type type);
std::ostream &operator<<(std::ostream &os, torricelly_system_type_type type);

}