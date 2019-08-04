#include <stdexcept>
#include <gasp/common/gasp_error.hpp>
#include <gasp/common/internal_error.hpp>

using namespace gasp::common;

gasp_error::gasp_error(const char *message) : logic_error(message) {}
gasp_error::gasp_error(const std::string &message) : logic_error(message) {}

gasp_internal_error::gasp_internal_error(const char *message) : logic_error(message) {}
gasp_internal_error::gasp_internal_error(const std::string &message) : logic_error(message) {}
