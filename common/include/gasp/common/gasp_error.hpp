#pragma once

#include <stdexcept>

namespace gasp::common
{

class gasp_error : public std::logic_error
{

public:
   gasp_error(const char *message);
   gasp_error(const std::string &message);
};

} // namespace gasp::common