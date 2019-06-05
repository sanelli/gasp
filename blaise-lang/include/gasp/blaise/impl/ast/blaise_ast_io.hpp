#pragma once

#include <ostream>
#include <vector>
#include <gasp/blaise/impl/ast/blaise_ast_type.hpp>

namespace gasp::blaise::ast {

std::ostream &operator<<(std::ostream &os, blaise_ast_type type);
std::ostream &operator<<(std::ostream &os, const std::vector<blaise_ast_type>& list);

}