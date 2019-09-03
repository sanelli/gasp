#pragma once

#include <string>
#include <stdexcept>

namespace gasp::blaise_to_torricelly
{

class blaise_to_torricelly_internal_error : public std::runtime_error
{

public:
   blaise_to_torricelly_internal_error(const char *message) : runtime_error(message) {}
   blaise_to_torricelly_internal_error(const std::string &message) : runtime_error(message) {}
};

class blaise_to_torricelly_error : public std::runtime_error
{
   int _line;
   int _column;

public:
   blaise_to_torricelly_error(int line, int column, const char *message) : runtime_error(message), _line(line), _column(column) {}
   blaise_to_torricelly_error(int line, int column, const std::string &message) : runtime_error(message), _line(line), _column(column) {}

   int line() const { return _line; }
   int column() const { return _column; }
};

} // namespace gasp::blaise_to_torricelly