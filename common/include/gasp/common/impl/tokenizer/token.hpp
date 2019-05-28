#pragma once

#include <string>

namespace gasp::common
{
template <typename TTokenType>
class token
{
   TTokenType _type;
   std::string _value;
   unsigned int _line;
   unsigned int _column;

public:
   token() : _type(static_cast<TTokenType>(0)), _value(""), _line(0), _column(0) {}
   token(TTokenType token, unsigned int line, unsigned int column) : _type(token), _value(""), _line(line), _column(column) {}
   token(TTokenType token, std::string value, unsigned int line, unsigned int column) : _type(token), _value(value), _line(line), _column(column) {}
   token(const token<TTokenType> &other) : _type(other._type), _value(other._value), _line(other._line), _column(other._column) {}
   token(token<TTokenType> &&other) : _type(std::move(other._type)), _value(std::move(other._value)), _line(std::move(other._line)), _column(std::move(other._column)) {}

   token<TTokenType>& operator=(const token<TTokenType>& other) {
      _type = other._type;
      _value = other._value;
      _line = other._line;
      _column = other._column;
      return *this;
   }

   token<TTokenType>& operator=(token<TTokenType>&& other) {
      _type = std::move(other._type);
      _value = std::move(other._value);
      _line = std::move(other._line);
      _column = std::move(other._column);
      return *this;
   }

   TTokenType type() const { return _type; }
   std::string value() const { return _value; }
   unsigned int line() const { return _line; }
   unsigned int column() const { return _column; }
};

}