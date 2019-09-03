#pragma once

#include <string>
#include <stdexcept>

namespace gasp::blaise::ast {

class blaise_ast_error : public std::runtime_error
{
   int _line;
   int _column;

public:
   blaise_ast_error(int line, int column, const char *message) : runtime_error(message), _line(line), _column(column) {}
   blaise_ast_error(int line, int column, const std::string &message) : runtime_error(message), _line(line), _column(column) {}

   int line() const { return _line; }
   int column() const { return _column; }
};

}