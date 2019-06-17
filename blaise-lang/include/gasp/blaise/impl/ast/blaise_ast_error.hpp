#pragma once

#include <string>
#include <stdexcept>

#include <gasp/blaise/tokens.hpp>
#include <gasp/common/tokenizer.hpp>

namespace gasp::blaise::ast {

class blaise_ast_error : public std::runtime_error
{
   int _line;
   int _column;

public:
   blaise_ast_error(int line, int column, const char *message) : runtime_error(message), _line(line), _column(column) {}
   blaise_ast_error(int line, int column, const std::string &message) : runtime_error(message), _line(line), _column(column) {}

   int line() { return _line; }
   int column() { return _column; }
};

}