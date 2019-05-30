#pragma once

#include <ostream>
#include <gasp/blaise/impl/language/blaise_language_type.hpp>

namespace gasp::blaise::language {

std::ostream &operator<<(std::ostream &os, blaise_language_type type);

}