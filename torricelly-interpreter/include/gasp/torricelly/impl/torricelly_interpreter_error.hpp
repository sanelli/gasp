#pragma once

#include <stdexcept>

namespace gasp::torricelly::interpreter
{
class torricelly_interpreter_error : public std::runtime_error
{

public:
   torricelly_interpreter_error(const char *message) : runtime_error(message) {}
   torricelly_interpreter_error(const std::string &message) : runtime_error(message) {}
};

class torricelly_interpreter_execution_error : public std::runtime_error
{
   std::string _subroutine;
   unsigned int _ip;

public:
   torricelly_interpreter_execution_error( 
         const std::string& subroutine,
         unsigned int ip,
         const std::string &message) :  runtime_error(message), _subroutine(subroutine), _ip(ip) {}

   std::string subroutine() const { return _subroutine; }
   unsigned int ip() const { return _ip; }

};

} // namespace gasp::torricelly::interpreter
