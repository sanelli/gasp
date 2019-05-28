#pragma once

#include <string>
#include <stdexcept>

namespace gasp::common
{

class parser_error : public std::domain_error
{
   int _line;
   int _column;

public:
   parser_error(const char *message) : domain_error(message), _line(0), _column(0) {}
   parser_error(std::string &message) : domain_error(message), _line(0), _column(0) {}
   parser_error(int line, int column, const char *message) : domain_error(message), _line(line), _column(column) {}
   parser_error(int line, int column, const std::string &message) : domain_error(message), _line(line), _column(column) {}

   int line() { return _line; }
   int column() { return _column; }
};

}
