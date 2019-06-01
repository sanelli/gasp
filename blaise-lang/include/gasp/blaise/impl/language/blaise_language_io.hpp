#pragma once

#include <ostream>
#include <vector>
#include <gasp/blaise/impl/language/blaise_language_type.hpp>

namespace gasp::blaise::language {

std::ostream &operator<<(std::ostream &os, blaise_language_type type);
std::ostream &operator<<(std::ostream &os, const std::vector<blaise_language_type>& list);

}