
#include <stdexcept>
#include <string>

#include <gasp/bf/bf-to-torricelly.hpp>

using namespace std;
using namespace gasp::bf;

bf_to_torricelly_error::bf_to_torricelly_error(const std::string& message) : domain_error(message) {}
bf_to_torricelly_error::bf_to_torricelly_error(const char *message) : domain_error(message) {}
