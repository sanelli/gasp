#pragma once

#include <string>
#include <stdexcept>

namespace gasp::blaise_to_torricelly
{

class blaise_to_torricelly_error : public std::runtime_error
{

public:
   blaise_to_torricelly_error(const char *message) : runtime_error(message) {}
   blaise_to_torricelly_error(const std::string &message) : runtime_error(message) {}
};

} // namespace gasp::blaise_to_torricelly