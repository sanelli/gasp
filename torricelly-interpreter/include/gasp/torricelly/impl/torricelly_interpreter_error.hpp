#pragma once

#include <stdexcept>

class torricelly_interpreter_error : public std::runtime_error
{

public:
   torricelly_interpreter_error(const char *message) : runtime_error(message) {}
   torricelly_interpreter_error(const std::string &message) : runtime_error(message) {}
};
