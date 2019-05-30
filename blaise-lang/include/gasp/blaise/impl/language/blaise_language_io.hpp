#pragma once

#include <ostream>
#include <gasp/blaise/impl/language/blaise_language_expression_type.hpp>

namespace gasp::blaise::language {

std::ostream &operator<<(std::ostream &os, blaise_expression_type type);

}