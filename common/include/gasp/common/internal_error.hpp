#include <stdexcept>
#include <exception>
#include <string>

namespace gasp::common
{

class gasp_internal_error : public std::logic_error
{

public:
   gasp_internal_error(const char *message) : logic_error(message) {}
   gasp_internal_error(const std::string &message) : logic_error(message) {}
};
} // namespace gasp::common