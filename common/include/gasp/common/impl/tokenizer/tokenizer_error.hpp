#pragma once

#include <string>
#include <stdexcept>


namespace gasp::common {

class tokenizer_error : public std::domain_error
{
   int _line;
   int _column;

public:
   tokenizer_error(int line, int column, const char *message) : domain_error(message), _line(line), _column(column) {}
   tokenizer_error(int line, int column, const std::string &message) : domain_error(message), _line(line), _column(column) {}

   int line() { return _line; }
   int column() { return _column; }
};
}