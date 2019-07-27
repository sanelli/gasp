#pragma once

#include <string>
#include <stdexcept>

namespace gasp::torricelly {

class torricelly_error : public std::runtime_error
{
public:
   torricelly_error(const char *message) : runtime_error(message) {}
   torricelly_error(const std::string &message) : runtime_error(message) {}
};

}