#pragma once

#include <stdexcept>

namespace gasp::common
{

class gasp_error : public std::logic_error
{

public:
   gasp_error(const char *message) : logic_error(message) {}
   gasp_error(const std::string &message) : logic_error(message) {}
};

} // namespace gasp::common