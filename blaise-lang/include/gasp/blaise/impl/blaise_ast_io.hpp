#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include <gasp/blaise/impl/blaise_ast_type.hpp>

namespace gasp::blaise::ast {

std::ostream &operator<<(std::ostream &os, blaise_ast_system_type type);
std::ostream &operator<<(std::ostream &os, std::shared_ptr<blaise_ast_type> type);
std::ostream &operator<<(std::ostream &os, const std::vector<std::shared_ptr<blaise_ast_type>>& list);

}